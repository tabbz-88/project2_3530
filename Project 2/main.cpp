#include "Taxon.h"
#include "CommonName.h"
#include "Relation.h"
#include <iostream>
#include <limits>
using namespace std;

int main() {
    cout << "Loading...\n";

    {

        Taxon taxon;
        CommonName common;

        if (!taxon.load("NameUsage.tsv")) return 1;
        if (!common.load("VernacularName.tsv")) return 1;


        string name1, name2, id1, id2;
        while (id1.empty() || id2.empty()) {
            cout << "\nEnter first species (common or scientific name): ";
            getline(cin, name1);
            cout << "Enter second species (common or scientific name): ";
            getline(cin, name2);

            id1 = common.getCommonID(name1);
            if (id1.empty()) id1 = taxon.getScientificID(name1);

            id2 = common.getCommonID(name2);
            if (id2.empty()) id2 = taxon.getScientificID(name2);

            if (id1.empty()) cout << name1 << " is not found in dataset.\n";
            if (id2.empty()) cout << name2 << " is not found in dataset.\n";
        }

        double relatedness = Relation::compute(taxon, id1, id2);
        string sci1 = taxon.getNode(id1)->name;
        string sci2 = taxon.getNode(id2)->name;
        string lca = Relation::findLCA(taxon, id1, id2);

        cout << "\nResults:\n";
        cout << " - " << name1 << " (" << sci1 << ")\n";
        cout << " - " << name2 << " (" << sci2 << ")\n";
        cout << " - Common ancestor: " << taxon.getNode(lca)->name << "\n";
        cout << " - Relatedness: " << relatedness << "%\n";
    }

    cout << "\nPress Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
