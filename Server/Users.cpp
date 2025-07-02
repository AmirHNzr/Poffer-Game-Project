#include "Users.h"

Users::Users(){
    AddUser(User("","",0,"","amir","A6xnQhbz4Vx2HuGl4lXwZ5U2I8iziLRFnhP5eNfIRvQ="));
    AddUser(User("","",0,"","amiri","A6xnQhbz4Vx2HuGl4lXwZ5U2I8iziLRFnhP5eNfIRvQ="));

}

Users::~Users(){}

bool Users::AddUser(const User& u)
{
    if(data.contains(u.getUserName()))
        return false;
    this->data.emplace(u.getUserName(),u);
    return true;
}

bool Users::EditUser(QString oldUsername,User newData)
{
    auto itrOld = data.find(oldUsername);
    if (itrOld == data.end()) {
        return false;
    }

    // 2) If the client isn't changing the username, do an in-place update:
    QString newUsername = newData.getUserName();
    if (newUsername == oldUsername) {
        itrOld->setFirstName(newData.getFirstName());
        itrOld->setLastName(newData.getLastName());
        itrOld->setEMail(newData.getEMail());
        itrOld->setPassword(newData.getPassword());
        itrOld->setPhoneNum(newData.getPhoneNum());
        return true;
    }

    // 3) Now the user really wants to change their username:
    if (data.contains(newUsername)) {
        return false;
    }

    // 4) Build a copy carrying over the old history, then insert/erase:
    User updatedUser = newData;
    updatedUser.setHistory(itrOld->getHistory());

    data.insert(newUsername, updatedUser);
    data.remove(oldUsername);
    return true;
}

QHash<QString, User> *Users::getData()
{
    return &data;
}

void Users::AddHistory(QString username,History hist)
{
    data[username].AddHistory(hist);
}

void Users::WriteFile(User u)
{
    QFile file("USERS_DATA.txt");
    if (!file.open(QIODevice::WriteOnly
                   | QIODevice::Append
                   | QIODevice::Text)) {
        qDebug() << "Couldn't open USERS_DATA.txt:" << file.errorString();
        return;
    }

    QTextStream out(&file);
    out << u.getUserName()  << "\n"
        << u.getFirstName() << "\n"
        << u.getLastName()  << "\n"
        << u.getEMail()     << "\n"
        << u.getPassword()  << "\n"
        << u.getPhoneNum()  << "\n";

    out << "{end}\n";

    QFileInfo info(file);
    qDebug() << "Absolute path:" << info.absoluteFilePath();
}

void Users::WriteHistory(QString u){
    qDebug() << "in write hist;";
    QFile file(data[u].getUserName()+"-Hist.txt");
    if (!file.open(QIODevice::WriteOnly
                   | QIODevice::Truncate
                   | QIODevice::Text)) {
        qDebug() << "Couldn't open" << file.errorString();
        return;
    }

    QTextStream out(&file);

    out << data[u].getHistory().size();
    for (const auto& h : data[u].getHistory()) {
        out << h.date.toString()  << "\n"
            << h.opponent << "\n"
            << h.result << "\n"
        << h.rounds[0] << "\n"
        << h.rounds[1] << "\n"
        << h.rounds[2] << "\n";
    }

    out << "{end}\n";

    QFileInfo info(file);
    qDebug() << "Absolute path:" << info.absoluteFilePath();
}

void Users::ReadFile()
{
    std::ifstream fileIn;
    fileIn.open("USERS_DATA.txt");
    if(!fileIn.is_open())
    {
        qDebug()<<"Couldn't open\n";
        return;
    }
    std::string line;
    while (true) {
        // 1) Read the six basic user fields
        std::string un, fn, ln, em, ps;
        std::string ph;

        if (!std::getline(fileIn, un))   break;               // username
        if (un.empty())                  continue;            // skip stray blank lines
        if (!std::getline(fileIn, fn))   break;               // first name
        if (!std::getline(fileIn, ln))   break;               // last name
        if (!std::getline(fileIn, em))   break;               // e-mail
        if (!std::getline(fileIn, ps))   break;               // password
        if (!std::getline(fileIn, ph)) break;               // phone as text

        // 2) Construct the User object
        User u;
        u.setUserName  (QString::fromStdString(un));
        u.setFirstName (QString::fromStdString(fn));
        u.setLastName  (QString::fromStdString(ln));
        u.setEMail     (QString::fromStdString(em));
        u.setPassword  (QString::fromStdString(ps));
        u.setPhoneNum  (QString::fromStdString(ph));

        // 4) Add the populated user to your container
        data.insert(QString::fromStdString(un),u);

        line="";
        if (std::getline(fileIn,line)){
            if(line=="{end}")
                continue;
            else
                break;}
        else
            break;
    }

    fileIn.close();
}

void Users::ReadHistory(){

    for(auto& user:data){

        std::ifstream fileIn;
        std::string name = user.getUserName().toStdString()+"-Hist.txt";
        fileIn.open(name);
        if(!fileIn.is_open())
        {
            qDebug()<<"Couldn't open\n";
            continue;
        }
        int hist;
        std::string history;
        std::string date,opp,res,r1,r2,r3;

        if(!std::getline(fileIn,history)) break;
        hist = std::stoi(history);

        while(hist != 0){
            if (!std::getline(fileIn, date))   break;
            if (!std::getline(fileIn, opp))   break;
            if (!std::getline(fileIn, res))   break;
            if (!std::getline(fileIn, r1))   break;
            if (!std::getline(fileIn, r2))   break;
            if (!std::getline(fileIn, r3)) break;

            user.AddHistory(History(QString::fromStdString(date),QString::fromStdString(opp)
                                    ,QString::fromStdString(res),QString::fromStdString(r1),
                                    QString::fromStdString(r2),QString::fromStdString(r3)));
        }


    }

}


