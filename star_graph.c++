/*
 * Parse the apache access log file and display the number of requests per hour
 * as a number and a graph
*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>

std::map<const std::string, int> buckets;

/*
 * 92.52.70.147 - - [17/Feb/2013:04:02:19 +0000] "GET /betfair_event/26961234 HTTP/1.1" 200 14626 "-" "-"
*/
void add_line_to_hash(std::string line)
{
    std::string key;
    int index;
    
    index = line.find('[');

    if(index > 0) {
        // Append the year
        key.append(line,index + 8, 4);
        key.append("-");

        // Append the month
             if(line.find("Jan") == index + 4) { key.append("01"); }
        else if(line.find("Feb") == index + 4) { key.append("02"); }
        else if(line.find("Mar") == index + 4) { key.append("03"); }
        else if(line.find("Apr") == index + 4) { key.append("04"); }
        else if(line.find("May") == index + 4) { key.append("05"); }
        else if(line.find("Jun") == index + 4) { key.append("06"); }
        else if(line.find("Jul") == index + 4) { key.append("07"); }
        else if(line.find("Aug") == index + 4) { key.append("08"); }
        else if(line.find("Sep") == index + 4) { key.append("09"); }
        else if(line.find("Oct") == index + 4) { key.append("10"); }
        else if(line.find("Nov") == index + 4) { key.append("11"); }
        else if(line.find("Dec") == index + 4) { key.append("12"); }
        else                                   { key.append("XX"); }
        key.append("-");

        // Append the day of the month
        key.append(line, index + 1, 2);
        key.append(" ");

        // Append the hour
        key.append(line, index + 13, 2);
        key.append(":--:--");
    
        buckets[key]++;
    }
}

void display_results()
{
    int max_value = 0;
    char stars[101];

    for(std::map<const std::string,int>::iterator it=buckets.begin(); it!=buckets.end(); ++it)
        if(max_value < it->second)
            max_value = it->second;

    for(std::map<const std::string,int>::iterator it=buckets.begin(); it!=buckets.end(); ++it) {
        int position = (int)(100.0 * ((double)it->second / (double)max_value));

        if(position == 0)
            position = 1;
        else if(position > 100)
            position = 100;

        for(int i = 0; i < position; ++i)
            stars[i] = '*';
        stars[position] = '\0';

        printf("%19s [%7d] (%7.2f) %s\n", it->first.c_str(), it->second, (double)it->second / 3600.0, stars);
    }
}

void read_from_file(char *filename)
{
    std::string line;
    std::ifstream myfile(filename);

    if(myfile.is_open()) {
        while(myfile.good()) {
            std::getline(myfile,line);
            add_line_to_hash(line);
        }
        myfile.close();
    }
}

void read_from_stdin()
{
    for(std::string line; std::getline(std::cin, line);)
        add_line_to_hash(line);
}

int main(int argc, char *argv[])
{
    if(argc == 1)
        read_from_stdin();
    else
        for(int i = 1; i < argc; ++i)
            read_from_file(argv[i]);

    display_results();

    return 0;
}
