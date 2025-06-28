#include "Users.h"

Users::Users(){}

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

void Users::WriteFile(User u)
{
    QFile file("USERS_DATA.txt");
    // Open for text‐only, write‐only
    if (!file.open(QIODevice::WriteOnly
                   | QIODevice::Append
                   | QIODevice::Text)) {
        qDebug() << "Couldn't open USERS_DATA.txt:" << file.errorString();
        return;
    }

    QTextStream out(&file);
    // No need to convert to std::string—QTextStream understands QString
    out << u.getUserName()  << "\n"
        << u.getFirstName() << "\n"
        << u.getLastName()  << "\n"
        << u.getEMail()     << "\n"
        << u.getPassword()  << "\n"
        << u.getPhoneNum()  << "\n";

    for (const auto& h : u.getHistory()) {
        out << h.date.toString()  << "\n"
            << h.opponent         << "\n"
            << h.result           << "\n"
            << h.rounds           << "\n";
    }

    out << "{end}\n";

    QFileInfo info(file);
    qDebug() << "Absolute path:"     << info.absoluteFilePath();
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
        int ph;

        if (!std::getline(fileIn, un))   break;               // username
        if (un.empty())                  continue;            // skip stray blank lines
        if (!std::getline(fileIn, fn))   break;               // first name
        if (!std::getline(fileIn, ln))   break;               // last name
        if (!std::getline(fileIn, em))   break;               // e-mail
        if (!std::getline(fileIn, ps))   break;               // password
        if (!std::getline(fileIn, line)) break;               // phone as text
        ph = std::stoi(line);

        // 2) Construct the User object
        User u;
        u.setUserName  (QString::fromStdString(un));
        u.setFirstName (QString::fromStdString(fn));
        u.setLastName  (QString::fromStdString(ln));
        u.setEMail     (QString::fromStdString(em));
        u.setPassword  (QString::fromStdString(ps));
        u.setPhoneNum  (ph);

        // 3) Read zero-or-more history entries until we hit "{end}"
        while (std::getline(fileIn, line) && line != "{end}") {
            // 'line' now holds the date-string
            std::string dateStr = line;
            std::string opp, res,rounds;

            if (!std::getline(fileIn, opp))  break;
            if (!std::getline(fileIn, res))  break;
            if (!std::getline(fileIn, line)) break;

            std::vector<History> h;
            History h1;
            h1.date     = QDateTime::fromString(QString::fromStdString(dateStr));  // uses Qt::TextDate by default
            h1.opponent = QString::fromStdString(opp);
            h1.result   = QString::fromStdString(res);
            h.push_back(h1);

            u.setHistory(h);   // or however you append to the user's history list
        }

        // 4) Add the populated user to your container
        data.insert(QString::fromStdString(un),u);
    }

    fileIn.close();
}


