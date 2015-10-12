#include <QCoreApplication>

extern void start_work();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    start_work();

    return a.exec();
}

