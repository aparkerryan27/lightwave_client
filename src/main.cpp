#include <easy_tcp.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace easy_tcp;
using namespace std;
using namespace chrono_literals;

struct MyData {
    int16_t r,g,b;
} d;

clock_t cp_start, cp_end;
double execution_time;

//MARK: recieve report information and send to data plotter
struct : Client {
    ofstream strm;
    int32_t data_count = 0; //TODO: make sure this doesn't overflow from extended use
    struct reportData {
        int16_t r, g, b = 0;
        friend std::ostream & operator << (std::ostream & out , const reportData & rd) {
            return out << rd.r << "," << rd.g << "," << rd.b;
        }
    };
    struct reportData *report = (reportData *)malloc(sizeof( struct reportData));
    int reportSize = sizeof(struct reportData);

    void received_data(char * b, size_t s) override {
        //write into report
        if (s != reportSize) { return; }
        memcpy(report, b, reportSize);
        //send out to virtual serial port that can be read by Arduino Plotter
        data_count += 1;
        cout << data_count << "," << *report << endl; //originally sent to strm;
        cp_end = clock();
        execution_time = ((double)(cp_end-cp_start))/CLOCKS_PER_SEC * 1000;
        cout << "roundtrip (ms): " << execution_time << endl;
    }
} c;

int main ( ) {
    //Clear the file
    ofstream temp;
    temp.open("data.csv", std::ofstream::out | std::ofstream::trunc);
    temp.close();

    //Write analytics data headers out
    c.strm.open("../data.csv", ios::out);
    c.strm << "count,r,g,b" << endl;

    //connect to node
    c.connect("10.0.0.154", 4500); //NOTE: change IP when on new network
    cout << "connected." << endl;
   
    for (int i = 0; i < 10; i++) {
        d.r = i * 10; //rand in the future but for now make it sequential so we can make sure data is sequential
        d.g = i * 10;
        d.b = i * 10;
        cp_start = clock();
        ((Connection) c).send_data((char *) &d, sizeof(d));
        cout << "sent data" << endl;

        this_thread::sleep_for(0.5s);
    }

    this_thread::sleep_for(20s);
    cout << "disconnecting";
    c.strm.close();

    c.disconnect();
}