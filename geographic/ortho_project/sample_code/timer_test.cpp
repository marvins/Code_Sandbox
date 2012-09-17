#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>

using namespace std;
using namespace boost::local_time;
using namespace boost::posix_time;
using namespace boost::gregorian;
using boost::posix_time::time_duration;

int main(){
    
    tzset();
    cout << tzname << endl;

    string reno_timezone = "PST-08";
    time_zone_ptr reno_tzp( new posix_time_zone(reno_timezone));

    //get the current time from the clock -- one second resolution
    ptime now = microsec_clock::universal_time();
    local_date_time _NOW( now, reno_tzp );
    
    cout << " PTIME: " << now << endl;
    cout << "  DATE: " << now.date() << endl;
    cout << "  TIME: " << now.time_of_day() << endl;
    cout << "String: " << to_simple_string(now) << endl;
    cout << "Local : " << _NOW << endl;
    sleep(1);
    ptime next = microsec_clock::universal_time();
    
    cout << endl;
    cout << "DIFF: " << next-now << endl;
    return 0;
}
