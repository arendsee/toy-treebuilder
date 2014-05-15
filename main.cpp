/* main.cpp
 * Zebulun Arendsee
 * May 9, 2014
 *
 * This is a stupid treebuilder. It works as follows:
 * 1) Remove all columns in the MSA that are fully conserved or contain gaps
 * 2) Build a distance matrix of the number of bases differing between each sequence 
 *
 * ASSUMPTION:
 * all sequences are highly similar (maybe >95%). In this case
 * double mutations are uncommon enough that they can be ignored.
 *
 * INPUT:
 * A single fasta file of aligned sequences. Each must be of equal length.
 */

#include <cstring>
using std::memset;
#include <stdlib.h>
#include <iostream>
using std::cout;
using std::endl;
using std::getline;
#include <fstream>
using std::ifstream;
#include <string>
using std::string;
#include <vector>
using std::vector;

void load_fasta(char infile[], vector<string> &seq, vector<string> &names);
vector<int> get_good_ids(vector<string> &seq);
bool char_in(char ch, char charset[], int n);

int main(int argc, char* argv[]){
    if(argc != 2){
        cout << "Damn you!" << endl;
        exit(1);
    }
    vector<string> seq;
    vector<string> names;
    load_fasta(argv[1], seq, names);

    int nseqs = names.size();

    vector<int> good_ids = get_good_ids(seq);

    int** counts = new int*[nseqs]; 
    for(int i = 0; i < nseqs; i++){
        counts[i] = new int[nseqs];
        memset(counts[i], 0, nseqs * sizeof(int));
    }

    int ngoods = good_ids.size();
    for(int i = 0; i < ngoods; i++){
        int seq_id = good_ids[i];
        for(int j = 0; j < nseqs; j++){
            for(int k = j + 1; k < nseqs; k++){
                if(seq[j][seq_id] != seq[k][seq_id]){
                    counts[j][k]++;
                }
            }
        }
    }

    /* Print the count table */
    cout << "acc" << '\t';
    for(int i = 0; i < nseqs; i++){
        cout << names[i][0] << '\t';
    }
    cout << endl;
    for(int i = 0; i < nseqs; i++){
        cout << names[i] << '\t';
        for(int j = 0; j < nseqs; j++){
            cout << counts[i][j] << '\t';
        }
        cout << endl;
    }

    /* Free memory */
    for(int i = 0; i < nseqs; i++){
        delete [] counts[i];
    }
    delete [] counts;
}

vector<int> get_good_ids(vector<string> &seq){
    vector<int> good;

    // acceptable characters in alphabet
    char alph[] = {'a', 'c', 'g' ,'t', 'A', 'C', 'G', 'T'};
    // length of alph
    int n_alph = 8;

    for(unsigned i = 0; i < seq[0].size(); i++){
        bool not_all_same = false;
        bool all_in_alph = true;
        char first = seq[0][i];
        for(unsigned j = 0; j < seq.size(); j++){
            if(seq[j][i] != first){
                not_all_same = true;
            }
            all_in_alph = char_in(seq[j][i], alph, n_alph);
            // Break loop if an outside-alphabet character is encountered
            if(!all_in_alph){
                break;
            }
        }
        // Add index iff 
        if(not_all_same && all_in_alph){
            good.push_back(i);
        }
    }
    return(good);
}

bool char_in(char ch, char charset[], int n){
    for(int i = 0; i < n; i++){
        if(ch == charset[i]){
            return(true);
        }
    }
    return(false);
}

void load_fasta(char infile[], vector<string> &seq, vector<string> &names){
    // load fasta file as array of arrays
    // check all are of equal length
    ifstream f (infile);
    if(!f.is_open()){
        cout << "Could not open file" << endl;
        exit(1);
    }
    string line;
    while(getline(f, line)){
        if(line[0] == '>'){
            names.push_back(line.substr(1, line.size()));
            seq.push_back("");
            // if(...){
            //     cout << "Seriously? Two identical headers?" << endl;
            //     exit(1);
            // }
        } else {
            seq.back() += line;
        }
    }

    /* Check whether any sequences were recognized */
    if(seq.size() == 0){
        cout << "Jeesh man, give me some real fasta sequence" << endl;
        exit(1);
    }

    /* Check whether all sequences are of equal length */
    unsigned* lengths = new unsigned[seq.size()];
    for(unsigned i = 0; i < seq.size(); i++){
       lengths[i] = seq[i].size();     
    }
    unsigned first_length = lengths[0];
    for(unsigned i = 1; i < seq.size(); i++){
        if(first_length != lengths[i]){
           cout << "Sequences must all be of equal length, duh" << endl; 
           exit(1);
        }
    }
    delete [] lengths;

    /* Check whether the sequence lengths are 0 */
    if(seq[0].size() == 0){
        cout << "I can't do jack with empty sequences" << endl;
    }
}
