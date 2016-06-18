#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include "synapse.h"

/*
 * when (neuron A, neuron B, weight)
 * when weight is positive, A is on then B is on
 * when weight is negative, A is off when B is on
 *
 */

using namespace std;

// global variables

//  file names for linux implementation
//string connectome_file = "connectome.csv";
//string synaptic_file = "postsynaptic.csv";
//string connectome_file = "edgelist.csv";
//string synaptic_file = "synaptic.csv";

//  direct file paths for debugging
//string connectome_file = "/Users/vanessaulloa/ClionProjects/connectome/connectome.csv";
//string synaptic_file = "/Users/vanessaulloa/ClionProjects/connectome/postsynaptic.csv";
//string connectome_file = "/Users/vanessaulloa/ClionProjects/connectome/edgelist.csv";
//string synaptic_file = "/Users/vanessaulloa/ClionProjects/connectome/synaptic.csv";

//string connectome_file = "K:\\School\\Summer_2016\\connectome_noMPI\\connectome.csv";
//string synaptic_file = "K:\\School\\Summer_2016\\connectome_noMPI\\postsynaptic.csv";
//string connectome_file = "K:\\School\\Summer_2016\\connectome_noMPI\\edgelist.csv";
//string synaptic_file = "K:\\School\\Summer_2016\\connectome_noMPI\\synaptic.csv";

string connectome_file = "C:\\Users\\Vanessa\\ClionProjects\\Connectome_Capstone_NoMPI\\connectome.csv";
string synaptic_file = "C:\\Users\\Vanessa\\ClionProjects\\Connectome_Capstone_NoMPI\\postsynaptic.csv";
//string connectome_file = "C:\\Users\\Vanessa\\ClionProjects\\Connectome_Capstone_NoMPI\\edgelist.csv";
//string synaptic_file = "C:\\Users\\Vanessa\\ClionProjects\\Connectome_Capstone_NoMPI\\synaptic.csv";

/*
 * threshold - determines when neuron fires
 * counter - for display
 */
int threshold = 15;
int neuronFireCount,muscleFireCount = 0;
string neuron;
clock_t t2;
ofstream outputfile;

//  vectors to hold neuron data
vector<synapse> connectome_vector;
vector<synapse> postsynaptic_vector;

//   function prototypes
void read_connectome();
void read_postsynaptic();
void dendriteAccumulate(synapse);
void fireNeuron(synapse);
void runconnectome(synapse);
void testFiles();

///

int main() {

    /*
        connectome_vector:
            full C Elegans Connectome
        postsynaptic_vector: 
            maintains accumulated values for each neuron and muscle.
    */

    /***** START USER INPUT *****/

    cout << "Please enter a neuron: ";
    cin >> neuron;

    /***** END USER INPUT *****/

    /***** OPEN FILE TO STORE SELECTED OUTPUT *****/

    //  get local time to append to file name for storage in output folder
    time_t t = time(NULL);
    tm* localTime = localtime(&t);

    int Day    = localTime->tm_mday;
    int Month  = localTime->tm_mon + 1;
    int Year   = localTime->tm_year + 1900;
    int Hour   = localTime->tm_hour;
    int Min    = localTime->tm_min;
    int Sec    = localTime->tm_sec;

    string outputDate = to_string(Day) + to_string(Month) + to_string(Year) + "_" + to_string(Hour) + to_string(Min) + to_string(Sec);

    //outputfile.open("/Users/vanessaulloa/ClionProjects/connectome_noMPI/output.txt");
    //outputfile.open("/Users/vanessaulloa/ClionProjects/Connectome_Capstone_NoMPI/output/" + neuron + "_" + outputDate + ".dat");
    //outputfile.open("K:\\School\\Summer_2016\\connectome_noMPI\\output\\"+ neuron + "_" + outputDate  + ".dat");
    outputfile.open("C:\\Users\\Vanessa\\ClionProjects\\Connectome_Capstone_NoMPI\\output\\"+ neuron + "_" + outputDate  + ".dat");
    //outputfile.open("output.txt");
    //outputfile.open("output/" + neuron + "_" + outputDate + ".dat");

    /***** END FILE DECLARATION *****/

    outputfile << "Please enter a neuron: ";
    outputfile << neuron << endl;


    /***** FILL VECTORS *****/

    read_connectome();
    read_postsynaptic();
    //testFiles(connectome_vector,postsynaptic_vector);

    /***** END FILL VECTORS *****/

    /*****  THREADING START *****/

    /*****  THREADING END *****/

    for (int i = 0; i < connectome_vector.size(); i++) {

        if (connectome_vector[i].get_neuronA() == neuron) {

            cout << "\n----------\n" << endl;
            cout << "Running Connectome with neuron: " << connectome_vector[i].get_neuronA()  << endl;
            cout << "----------\n" << endl;

            outputfile << "\n----------\n" << endl;
            outputfile << "Running Connectome with neuron: " << connectome_vector[i].get_neuronA()  << endl;
            outputfile << "----------\n" << endl;

            //  pass neuron from user input to runconnectome function
            runconnectome(connectome_vector[i]);

        }
    }

    /***** END OF PROGRAM DATA *****/

    t2 = clock();

    cout << "\n----------\n";
    cout << "Total Neurons Fired: " << neuronFireCount << endl;
    cout << "Total Muscles Fired: " << muscleFireCount << endl;
    cout << "Total Run Time: " << (double)(t2/CLOCKS_PER_SEC) << " seconds" << endl;
    cout << "----------\n";

    outputfile << "\n----------\n";
    outputfile << "Total Neurons Fired: " << neuronFireCount << endl;
    outputfile << "Total Muscles Fired: " << muscleFireCount << endl;
    outputfile << "Total Run Time: " << (double)(t2/CLOCKS_PER_SEC) << " seconds" << endl;
    outputfile << "----------\n";

    /***** END OF PROGRAM DATA END *****/

    //  close the filestream
    outputfile.close();

    return 0;

}// end main

/*      FUNCTIONS       */

/*
 * testFiles()
 * tests that the vectors were filled with data from .csv
 * by displaying the vector size and vector contents
 *
 */

void testFiles()  {

    /*****  DISPLAY CONNECTOME VECTOR *****/

    cout << "connectome vector size: " << connectome_vector.size() << endl;

    for(int i = 0; i < connectome_vector.size(); i++) {
        cout << "\t" << i << " : " << connectome_vector[i].get_neuronA() << endl;
        cout << "\t" << i << " : " << connectome_vector[i].get_neuronB() << endl;
        cout << "\t" << i << " : " << connectome_vector[i].get_weight() << endl;
        cout << endl;
    }

    /*****  DISPLAY POSTSYNAPTIC VECTOR *****/

    cout << "postsynaptic vector size: " << postsynaptic_vector.size() << endl;

    for(int i = 0; i < postsynaptic_vector.size(); i++) {
        cout << "\t" << i << " : " << postsynaptic_vector[i].get_neuronA() << endl;
        cout << "\t" << i << " : " << postsynaptic_vector[i].get_weight() << endl;
        cout << endl;
    }

}

/*
 * read_connectome()
 *
 */
void read_connectome() {

    // create file reading object and open the file
    /*  NEED WAY TO DYNAMICALLY REFERENCE file  */
    ifstream file;
    file.open(connectome_file);

    // substring variables
    string line;
    size_t pos1, pos2,length;
    string neuronA,neuronB,weight,temp;

    //  continue while file is open

    if(!file.is_open()) {

        cout << " Connectome.csv file could not be opened. " << endl;
        outputfile << " Connectome.csv file could not be opened. " << endl;
        exit(0);

    }   else    {

        while (getline(file, line)) {

            /*
             * int pos1 = first position of ',' in value
             * string neuronA = from 0 to pos1 in value
             * string temp = pos1 to length
             * int pos2 = fiirst position of ',' in temp
             * string neuronB = from pos1 to pos2 in value
             * string weight = from pos2 to string_length in value
             *
             */

            length = line.length();
            pos1 = line.find(',') + 1;

            neuronA = line.substr(0,pos1 - 1);
            temp = line.substr(pos1,length);
            pos2 = temp.find(',');

            neuronB = temp.substr(0,pos2);
            weight = temp.substr(pos2 + 1,length);
            int w = atoi(weight.c_str());

            //  push a new synapse object onto the vector
            connectome_vector.push_back(synapse(neuronA,neuronB,w));

        }   //end while loop

        cout << "\nFile " << connectome_file << " successfully read." << endl;
        outputfile << "\nFile " << connectome_file << " successfully read." << endl;

    }// end if statement

    //  close the file
    file.close();

}// end function

/*
 * read_postsynaptic()
 *
 */
void read_postsynaptic()    {

    // create file reading object and open the file
    /*  NEED WAY TO DYNAMICALLY REFERENCE file  */
    ifstream file;
    file.open(synaptic_file);

    // substring variables
    string line,neuronA;
    size_t pos1;

    if(!file.is_open()) {

        cout << "file could not be opened. " << endl;
        outputfile << "file could not be opened. " << endl;
        exit(0);

    }   else    {

        while(getline(file,line)) {

            pos1 = line.find(',') + 1;
            neuronA = line.substr(0,pos1 - 1);

            //cout << "neuronA: " << neuronA << endl;

            //  push a new synapse object onto the vector
            postsynaptic_vector.push_back(synapse(neuronA, 0));

        }// end while

        cout << "File " << synaptic_file << " successfully read.\n" << endl;
        outputfile << "File " << synaptic_file << " successfully read.\n" << endl;

    }//  end if statement

    file.close();

}// end function

/*
 * dendriteAccumulate()
 *
 */
void dendriteAccumulate(synapse a)  {

    int x,y;

    for(x = 0 ; x < connectome_vector.size() ; x++) {

        if(connectome_vector[x].get_neuronA() == a.get_neuronA())    {

            for(y = 0; y < postsynaptic_vector.size() ; y++)    {

                if(postsynaptic_vector[y].get_neuronA() == connectome_vector[x].get_neuronB())   {

                    postsynaptic_vector[y].set_weight(connectome_vector[x].get_weight());
                    /*
                    cout << "\tpostsynaptic weight: ";
                    cout << " " << postsynaptic_vector[y].get_neuronA() << ", " << (postsynaptic_vector[y].get_weight() - connectome_vector[x].get_weight()) << "+" << connectome_vector[x].get_weight() << " = " << postsynaptic_vector[y].get_weight() << endl;
                    outputfile << "\tpostsynaptic weight: ";
                    outputfile << " " << postsynaptic_vector[y].get_neuronA() << ", " << (postsynaptic_vector[y].get_weight() - connectome_vector[x].get_weight()) << "+" << connectome_vector[x].get_weight() << " = " << postsynaptic_vector[y].get_weight() << endl;
                    */
                }// end if

            }// end for

        }// end if

    }// end for

}// end dendriteAccumulate()

/*
 * fireNeuron()
 * when the threshold has been exceeded, fire the neurite
 * 1st we accumulate the postsynaptic weights
 * then we check everywhere the accumulator is > threshold
 *
 */
void fireNeuron(synapse a)   {

    int y;
    dendriteAccumulate(a);

    for(y = 0 ; y < postsynaptic_vector.size() ; y++ )   {

        if(abs(postsynaptic_vector[y].get_weight()) > threshold)  {

            if(postsynaptic_vector[y].get_neuronA().substr(0,2) == "MV"
               || postsynaptic_vector[y].get_neuronA().substr(0,2) == "MD"
               || postsynaptic_vector[y].get_neuronA() == "PLMR"
               || postsynaptic_vector[y].get_neuronA() == "PLML") {

                muscleFireCount++;
                cout << "Fire Muscle " + postsynaptic_vector[y].get_neuronA() << abs(postsynaptic_vector[y].get_weight()) << endl;
                outputfile << "Fire Muscle " + postsynaptic_vector[y].get_neuronA() << abs(postsynaptic_vector[y].get_weight()) << endl;
                postsynaptic_vector[y].reset_weight();

            } else {

                neuronFireCount++;
                cout << "Fire Neuron " + postsynaptic_vector[y].get_neuronA() << endl;
                outputfile << "Fire Neuron " + postsynaptic_vector[y].get_neuronA() << endl;
                dendriteAccumulate(postsynaptic_vector[y]);
                postsynaptic_vector[y].reset_weight();

            }// end if/else

        }// end if

    }// end for

}// end fireNeuron()

/*
 * runconnectome()
 *
 *
 */
void runconnectome(synapse a)   {

    int y;

    dendriteAccumulate(a);

    for(y = 0 ; y < postsynaptic_vector.size() ; y++)    {

        if(abs(postsynaptic_vector[y].get_weight()) > threshold)  {

            fireNeuron(postsynaptic_vector[y]);
            postsynaptic_vector[y].reset_weight();

        }// end if

    }// end for

}// end runconnectome()
