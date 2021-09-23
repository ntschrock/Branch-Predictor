#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

int ExtractBit(int, int, int);

int main(int argc, char ** argv) {

	ifstream inFile;

	string format_Str;
	long long int pc_Address;
	long long int ind;
	int GHB = 0;
	int* branch_Table;
	char output;
	int entries_number;
	int i = 0;

	//final stats
	int miss_predicts = 0;
	int total_predicts = 0;
	float miss_ratio = 0;

	int m = atoi(argv[1]);
	int n = atoi(argv[2]);

	inFile.open(argv[3]);

	entries_number = pow(2, m);
	branch_Table = new int[entries_number];

	if (!inFile) {
	
		cout << "input file was unsuccessfully opened terminating program." << endl;
		exit(0);
	}

	for (i = 0; i < entries_number; i++) {
	
		branch_Table[i] = 1; // initialize to weakly taken
	}
	while (!inFile.eof()) {

		inFile >> format_Str;
		inFile >> output;

		pc_Address = stoi(format_Str, nullptr, 16);


		//remove the least significant 2 bits
		pc_Address = pc_Address >> 2;

		//take the M least significant bits from the new PC Address

		pc_Address = ExtractBit(pc_Address, m, 1);


		// XOR the results 
		ind = (pc_Address ^ (GHB << (m - n)));
		

		//update GHB, 
		//update branch_Table entry
		//update final states

		if(branch_Table[ind]== -1) // strongly not taken
        {

			if (output == 'n' || output == 'N') {

				total_predicts++;
				GHB = GHB >> 1;
			}

			else if (output == 't' || output == 'T') {

				total_predicts++;
				miss_predicts++;
				GHB = GHB >> 1;
				GHB = GHB + pow(2, n - 1);
				branch_Table[ind] = 0; // weakly not taken

			}
            
        }

		else if(branch_Table[ind]== 0) // weakly not taken
        {
			if (output == 'n' || output == 'N') {

				total_predicts++;
				GHB = GHB >> 1;
				branch_Table[ind] = -1; // weakly not taken to strongly not taken
			}

			if (output == 't' || output == 'T') {

				total_predicts++;
				miss_predicts++;
				GHB = GHB >> 1;
				GHB = GHB + pow(2, n - 1);
				branch_Table[ind] = 1; // weakly taken to weakly taken
			}
        }
        
		else if(branch_Table[ind]== 1) // weakly taken
        {
			if (output == 'n' || output == 'N') {


				total_predicts++;
				miss_predicts++;
				GHB = GHB >> 1;
				branch_Table[ind] = 0; //weakly taken to weakly not taken

			}

			if (output == 't' || output == 'T') {

				total_predicts++;
				GHB = GHB >> 1;
				GHB = GHB + pow(2, n - 1);
				branch_Table[ind] = 2; // weakly taken to strongly taken

			}
        }

		else if(branch_Table[ind]== 2) // strongly taken
        {
			if (output == 'n' || output == 'N') {


				total_predicts++;
				miss_predicts++;
				GHB = GHB >> 1;
				branch_Table[ind] = 1;
			}

			if (output == 't' || output == 'T') {


				total_predicts++;
				GHB = GHB >> 1;
				GHB = GHB + pow(2, n - 1);
			}
        }
        else
        {
			cout << "the default case" << endl;
			
        }
    }
	
	cout << dec << fixed << "M: " << m << " N: " << n;

	miss_ratio = miss_predicts / (float)total_predicts;
	cout << setprecision(2) << " Miss ratio: " << float(miss_ratio)*100 << "%"<< endl;
	
	inFile.close();
	delete[] branch_Table;
	return 0;
}

int ExtractBit(int n, int l, int m){

	return (((1 << l) - 1) & (n >> (m - 1)));
}
