#include <QApplication>
#include "json_visualizer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    JsonVisualizer visualizer;
    visualizer.show();
    
    return app.exec();
}
