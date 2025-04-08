#include "bcrypt.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{

    string password = "top_secret";

    string hash = bcrypt::generateHash(password);

    cout << "Hash: " << hash << std::endl;

    cout << "\"" << password << "\" : " << bcrypt::validatePassword(password,hash) << std::endl;
    cout << "\"wrong\" : " << bcrypt::validatePassword("wrong",hash) << std::endl;

    return 0;
}
