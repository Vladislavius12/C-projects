#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include "Bus_Table_Builder.h"

void  SortData(std::vector<Bus_Table>& v);

int main() {
    std::vector<Bus_Table> data; //Vector with data from the input file
    std::vector<Bus_Table> right_data;

    std::ifstream in("Bus_table.txt");
    std::ofstream out("Output.txt");

    if (!in)
    {
        std::cout << "Error file not found" << std::endl;
        return -1;
    }
    if (!out)
    {
        std::cout << "Error creating file" << std::endl;
        return -2;
    }
    char str[256];

    Bus_Table_Builder builder;

    while (!in.eof())
    {
        char Company_name[10];
        char depart_time_h[3];
        char depart_time_m[3];
        char arrival_time_h[3];
        char arrival_time_m[3];

        in.getline(Company_name, 256, ' ');
        in.getline(depart_time_h, 256, ':');
        in.getline(depart_time_m, 256, ' ');
        in.getline(arrival_time_h, 256, ':');
        in.getline(arrival_time_m, 256, '\n');


        data.push_back(builder.fullfil(Company_name, depart_time_h, depart_time_m, arrival_time_h, arrival_time_m));
    }

    for (int i = 0; i < (int)data.size(); i++) {
        Bus_Table first;
        first = data.at(i); //It is better to initialize as links
        if (first.getnArrival_time() - first.getDepart_time() <= 60) { //Check 1 hour requirement
            right_data.push_back(first);
        }
        /*
        if (first.getnArrival_time() - first.getDepart_time() <= 60) {
            if (first.getDepart_time() == second.getDepart_time()) {
                if (first.getnArrival_time() < second.getnArrival_time()) { //Starts earlier
                    right_data.push_back(first);
                }
                else if (first.getnArrival_time() > second.getnArrival_time()) {
                    right_data.push_back(second);
                }
                else if (first.getnArrival_time() == second.getnArrival_time()) {
                    if (std::string{ "Posh" }.compare(first.getCompany_name()) == 0) {
                        right_data.push_back(first);
                    }
                    else {
                        right_data.push_back(second);
                    }

                }
            }
            else if (first.getDepart_time() > second.getDepart_time()) { //Starts later
                if (first.getnArrival_time() < second.getnArrival_time()) {
                    right_data.push_back(first);
                }
            }
            else if (first.getDepart_time() < second.getDepart_time()) {
                if (first.getnArrival_time() < second.getnArrival_time()) {
                    right_data.push_back(first);
                }
            }


        }
       */
    }

    data = right_data;
    right_data.clear();
   

    for (int j = 0; j < (int)data.size(); j++) {
        bool eff = true;
        Bus_Table second = data.at(j);
        
        for (int i = 0; i < (int)data.size(); i++) {
            
            Bus_Table first;
            first = data.at(i);
            
            if (i == j) continue;
            if (second.getDepart_time() == first.getDepart_time()) {
                if (second.getnArrival_time() > first.getnArrival_time()) {
                    eff = false;
                }
                else if (second.getnArrival_time() == first.getnArrival_time()) {
                    if (std::string{ "Grotty" }.compare(second.getCompany_name()) == 0) {
                        eff = false;
                    }
                }
            }
            else  if (second.getnArrival_time() == first.getnArrival_time()) {
                if (second.getDepart_time() < first.getDepart_time()) {
                    eff = false;
                }
            }
            else  if (second.getDepart_time() < first.getDepart_time()) {
                if (second.getnArrival_time() > first.getnArrival_time()) {
                    eff = false;
                }
            }
            
        }
        if (eff) {
            right_data.push_back(second);
        }
    }





        SortData(right_data);

        for (int i = 0; i < (int)right_data.size(); i++) {
            Bus_Table first;
            first = right_data.at(i);
            if (std::string{ "Posh" }.compare(first.getCompany_name()) == 0) {
                out << right_data[i].toString() << std::endl;
            }
        }
             out<< std::endl;

        for (int i = 0; i < (int)right_data.size(); i++) {
            Bus_Table first;
            first = right_data.at(i);
            if ("Grotty" == first.getCompany_name()) { // (std::string{ "Grotty" }.compare(first.getCompany_name()) == 0)
                out << right_data[i].toString() << std::endl;
            }
        }
    
}

void  SortData(std::vector<Bus_Table>& v) {
    for (int i = 0; i < (int)v.size() - 1; i++) {
        for (int j = 0; j < (int)v.size() - 1 - i; j++) {
            if (v[j].getDepart_time() > v[j + 1].getDepart_time()) {
                Bus_Table t = v[j];
                v[j] = v[j + 1];
                v[j + 1] = t;
            }
        }
    }
}