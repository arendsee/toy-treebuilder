/* Count class
 * Stores distance matrix
 */

#ifndef COUNT_H
#define COUNT_H

#include <vector>
using std::vector;

class Count
{
public:
    Count(vector<string> names); 
    string get_name(int i);
private:
    vector< vector<int> > counts;
    vector<string> names;
}

Count::Count(vector<string> names){
    int N = names.size();
    Count::names = names;
}

string Count::get_name(int i){
    return(Count::names[i]);
}

#endif
