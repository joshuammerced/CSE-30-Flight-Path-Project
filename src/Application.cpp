#include "ArrayList.h"
#include "Graph.h"
#include <Application.h>
#include <FL/Enumerations.H>
#include <FL/Fl_Scroll.H>
#include <bobcat_ui/all.h>
#include <bobcat_ui/dropdown.h>
#include <bobcat_ui/textbox.h>
#include <fstream>
#include <sstream>
#include <string>

using namespace bobcat;
using namespace std;

Application::Application() {
    initData();
    initInterface();
}

void Application::handleClick(bobcat::Widget *sender) {

    results->clear();
    window->redraw();

    int startIndex = start->value();
    int destIndex = dest->value();
    int prefPath = pref->value();
    Waypoint *path = nullptr;

    if( prefPath == 0){
        path = g.ccs(cities[startIndex], cities[destIndex]);
        system("clear");
        cout<<"Searching Cheapest Price..."<< endl;


    }
    else if( prefPath == 1){
        path = g.ucs(cities[startIndex], cities[destIndex]);
        system("clear");
        cout<<"Searching Shortest Trip..."<<endl;
    }
    else {
        path = g.bfs(cities[startIndex], cities[destIndex]);
        cout<<"Searching Least Stops..."<<endl;
    }
    system("clear");
    if (path) {
        ArrayList<Waypoint *> pathList;
        Waypoint *temp = path;
        while (temp != nullptr) {
            pathList.append(temp);
            temp = temp->parent;
        }
        int j = 0;
        int y = results->y() + 10;
        for (int i = pathList.size()-1; i>=0; i--){
            Waypoint *point = pathList[i];
            results->add(new TextBox(40, y, 300, 25,point->vertex->data));
            if (i>0){
                Waypoint *next= pathList[i-1];
                string info;
                if(prefPath == 1){
                    info = "Flight Time: " + to_string(next->weight) + " hours";
                }
                else if(prefPath == 0){
                    int flightCost = next->totalCost - point->totalCost;
                    info = "Cost: $" + to_string(flightCost);
                }
                else{
                    j++;
                    info ="Stops: "+to_string(j);
                }
                y += 20;
                results->add(new TextBox(40,y,300,25, info));
                y += 20;
                results->add(new TextBox(40,y,300,25,"|"));
                y += 12;
                results->add(new TextBox(38,y,300,25,"V"));
                y += 20;

            }
        }
            int totalStop = pathList.size()-1;
            totalC->label("Total Cost: $" + to_string(path->totalCost));
            totalS->label("Total Stops: " + to_string(totalStop));
            totalT->label("Total Time: " + to_string(path->partialCost) + " hours");
            window->redraw();
    } else {
        cout << "There is no path" << endl;
        results->add(new TextBox(40,results->y() + 10,300,25,"There is no path"));

    }

}

void Application::initData() {
    fstream file("assets/vertices.csv");
    if (file.is_open()){
        string line;
        while(getline(file,line)){
            if(!line.empty()){
                cities.append(new Vertex(line));
            }
        }
        file.close();
    }

    for (int i = 0; i < cities.size(); i++) {
        g.addVertex(cities[i]);
    }

    fstream efile("assets/edges.csv");
    if (efile.is_open()){
        string line;
        while(getline(efile,line)){
            if(!line.empty()){
            string from , to , wei , $$;
            stringstream lp(line);
            getline(lp,from,',');
            getline(lp,to,',');
            getline(lp,wei,',');
            getline(lp,$$,',');

            int ifrom = stoi(from);
            int ito = stoi(to);
            int lbs = stoi(wei);
            int mula = stoi($$);
            
            if(ifrom >= 0 && ifrom < cities.size() && ito >= 0 && ito < cities.size()){
                g.addEdge(cities[ifrom], cities[ito], lbs, mula);
            }
            }
        }
        efile.close();
    }

}

void Application::initInterface() {
    window = new Window(100, 100, 400, 400, "Flight Planner");

    start = new Dropdown(20, 30, 360, 25, "Starting Point");
    dest = new Dropdown(20, 80, 360, 25, "Destination");
    pref = new Dropdown(20,130,360,25,"Preference");
    pref->add("Cheapest price");
    pref->add("Shortest travel time");
    pref->add("Least number of stops");
    for (int i = 0; i < cities.size(); i++) {
        start->add(cities[i]->data);
        dest->add(cities[i]->data);
    }
    search = new Button(20, 175, 360, 25, "Search");
    ON_CLICK(search, Application::handleClick);

    totalC = new TextBox(20,335,360,25, "Total Cost: ");
    totalS = new TextBox(20,355,360,25,"Total Stops: ");
    totalT = new TextBox(20,375,360,25, "Total Time: ");

    results = new Fl_Scroll(20, 230, 360, 100, "Results");
    results->align(FL_ALIGN_TOP_LEFT);
    results->box(FL_THIN_UP_BOX);


    window->show();
}
