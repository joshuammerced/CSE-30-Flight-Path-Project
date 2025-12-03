#ifndef APPLICATION_H
#define APPLICATION_H

#include <bobcat_ui/all.h>
#include <Graph.h>
#include <FL/Fl_Scroll.H>
#include <bobcat_ui/dropdown.h>
#include <bobcat_ui/textbox.h>


class Application : public bobcat::Application_ {
    bobcat::Window *window;
    bobcat::Dropdown *start;
    bobcat::Dropdown *dest;
    bobcat::Dropdown *pref;
    bobcat::TextBox *totalS;
    bobcat::TextBox *totalC;
    bobcat::TextBox *totalT;
    bobcat::Button *search;

    Fl_Scroll *results;

    ArrayList<Vertex*> cities;
    Graph g;

    void initData();
    void initInterface();

    void handleClick(bobcat::Widget *sender);
public:
    Application(); // Constructor for the app
};

#endif