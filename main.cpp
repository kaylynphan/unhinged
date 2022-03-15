//main.cpp

#include "PersonProfile.h"
#include "AttributeTranslator.h"
#include "MemberDatabase.h"
#include "MatchMaker.h"
#include "provided.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const string MEMBERS_FILE    = "members.txt";
const string TRANSLATOR_FILE = "translator.txt";

bool findMatches(const MemberDatabase& mdb, const AttributeTranslator& at);

int main() {
    MemberDatabase mdb;
    if (!mdb.LoadDatabase(MEMBERS_FILE))
    {
        cout << "Error loading " << MEMBERS_FILE << endl;
        return 1;
    }
    AttributeTranslator at;
    if (!at.Load(TRANSLATOR_FILE))
    {
        cout << "Error loading " << TRANSLATOR_FILE << endl;
        return 1;
    }
    /*
    while (findMatches(mdb, at))
        ;
        */

    findMatches(mdb, at);

    cout << "Happy dating!" << endl;
}

bool findMatches(const MemberDatabase& mdb, const AttributeTranslator& at)
{
      // Prompt for email
    string email;
    const PersonProfile* pp;
    for (;;) {
        cout << "Enter the member's email for whom you want to find matches: ";
        getline(cin, email);
        if (email.empty())
            return false;
        pp = mdb.GetMemberByEmail(email);
        if (pp != nullptr)
            break;
        cout << "That email is not in the member database." << endl;
    }

      // Show member's attribute-value pairs
    cout << "The member has the following attributes:" << endl;
    for (int k = 0; k != pp->GetNumAttValPairs(); k++) {
        AttValPair av;
        pp->GetAttVal(k, av);
        cout << av.attribute << " --> " << av.value << endl;
    }

      // Prompt user for threshold
    int threshold;
    cout << "How many shared attributes must matches have? ";
    cin >> threshold;
    cin.ignore(10000, '\n'); 

      // Print matches and the number of matching translated attributes
    MatchMaker mm(mdb, at);
    vector<EmailCount> emails = mm.IdentifyRankedMatches(email, threshold);
    if (emails.empty())
        cout << "No member was a good enough match." << endl;
    else {
        cout << "The following members were good matches:" << endl;;
        for (const auto& emailCount : emails) {
            const PersonProfile* pp = mdb.GetMemberByEmail(emailCount.email);
            cout << pp->GetName() << " at " << emailCount.email << " with "
                      << emailCount.count << " matches!" << endl;
        }
    }
    cout << endl;
    return true;
}
