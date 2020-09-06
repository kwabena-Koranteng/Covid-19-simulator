#include <iostream>
#include<string>
#include <fstream>
#include<random>
using namespace std;


/**
 * This function is a simulation function which takes in a float input and returns true or false depending on the input
*/
bool simulate_prob(float prob){

  prob = prob*1000;

  random_device dev;
  mt19937 rng(dev());
  uniform_int_distribution<mt19937::result_type> random_number(1,1000); // distribution in range [1, 1000]

  if ( random_number(rng) <= prob)
    return true;

  return false;
}


enum disease_status {vulnerable , infected , sick , dead};       //This helps keep track of the person's status


    double infect=0.0;                                            //Keeps track of the number of infected people
    double gone=0.0;                                             //Keeps track of the number of dead people
    double recov=0.0;                                            //Keeps track of the number of people who recovered
    int sickk=0;                                                //Keeps track of the number of sick people
    int si=0;                                                  //Keeps track of the number of people who are still sick and the end of the simulation.
    double death_r=0.0;                                         //Keeps track of the death rate. death rate is calcluated as the dead/Population
    double recover_r=0.0;                                        //Keeps track of the recovery rate.It is calculated as recovered/infected
    int average_d=0;                                              //Keeps track of the average death age.
    int ma=0;





/**
 * This function takes a file input and reads the number of lines in the file. It is later passed into the graph to enable it create the exact number of nodes needed.
 */
int numLines(string filename) {
    int count=0;
   ifstream file;
   string word;
   file.open(filename.c_str());
   while(!file.eof()) {
     getline(file,word);
     count++;
   }
   file.close();
   return count;
}

/**
 * This function takes a file input and finds the maximum age in the population.
 */
int maxage(string filename){
   string name;
   float spreadP;
   int max=0;
   float age;
   fstream file;
   file.open(filename.c_str());
   while(file >>name>> age>>spreadP ){
      if (age>max)
		     max=age;
   }
   return max;

}

/**
 * This is an edge struct which has a pointer to the next edge and has a destination stored as int to help in indexing of the next node.
 */
struct Edge{
int destination;
Edge* nPtr;
};

/**
 * This is a node struct and it stores everything there is to know about a single node. It uses the name of the nodes as a reference point and has a list of edge pointers (If the edge exist)
 */
struct Node{
string name;
float age;
float spreadP;
double diseaseP;
float recoveryR;
disease_status status;
int numDays=0;
bool visit=false;


Edge* edPtr;
};

/**
 * This is the Graph class which has different functions to help populate it.
 */
class Graph{
private:
Node* nodes;
int NoV;

public:
Graph(int num):nodes(NULL),NoV(num){                                                             //The constructor that takes in num as an argument and creates nodes based on that number
if (num){
        nodes= new Node[num];
}

}

~Graph(){                                                                                       //Destructor which deallocates all edge and node list
    delete[] nodes;
}

/**
 *This function adds a node into a graph
 */

void insert_node(string n , float l, float d, float s, float p)
{

for (int i =0 ; i<= NoV ; i++){
    if (nodes[i].name.empty()){
    nodes[i].name =n;
    nodes[i].age=l;
    nodes[i].spreadP=d;
    nodes[i].diseaseP=s;
    nodes[i].recoveryR=p;
    nodes[i].status = vulnerable;
    nodes[i].edPtr=NULL;
    break;
    }

}
}

/**
 *This function takes a filename as an argument and adds nodes into the graph
 */
void insertNode(string filename){
    int q=maxage(filename);
    string name;
    float age;
    float spreadP;
    fstream file;
    file.open(filename.c_str());
    while(file >> name >> age >> spreadP){
        float s =age/q;
        float q =1 -s;
         insert_node(name , age, spreadP,s,q);

    }

}
/**
 *This function adds an edge between a a source node and destination node
 */

void insert_Edge(string s , string d)
{
int i = IndexOf(s);
int j = IndexOf(d);

Edge* edge = nodes[i].edPtr;
nodes[i].edPtr = new Edge;
nodes[i].edPtr->nPtr =edge;
nodes[i].edPtr->destination = j;

}
/**
 *This function takes a filename as an argument and adds an edge between two nodes
 */


void insertEdge(string filename){
    string str1;
    string str2;

    fstream file;
    string word;
    file.open(filename.c_str());
    while(file >> str1 >> str2){
        insert_Edge(str1,str2);
        insert_Edge(str2,str1);
    }
}

/**
 *This function takes in a string argument and returns their index.
 */
int IndexOf(string v){
    for(int i=0 ; i<= NoV ;i++){
    if(nodes[i].name==v){return i;}

    }
    return -1;
}

/**
 *This function prints the edge list representation of the connections in your population
 */
void Print(){
    for (int i =0 ; i<+ NoV ; i++){
    cout<<nodes[i].name<<endl;
     cout <<"^^^^^^^^^^^^^"<<endl;
    if(!nodes[i].edPtr){
    cout<<endl;

        continue;
    }
    Edge* edge= nodes[i].edPtr;
    while(edge!=NULL){

        cout<< nodes[edge->destination].name<<endl;
        edge=edge->nPtr;

    }
    cout<<"-----------------"<<endl;
    cout<< endl;
    }
}
/**
 *This function prints the details of every node.
 */


void Printdetails()
{
    for(int i=0; i<NoV ; i++){
        cout<< nodes[i].name<< "\t" <<nodes[i].status  << "\t" << nodes[i].spreadP<<endl;
    }

}
/**
 *This function is responsible for the simulation of the infections in the population.
 */
  void simulation(){

   srand(time(0));
   int randomNumber;

   randomNumber=(rand()%NoV);
   string Patient=nodes[randomNumber].name;
   nodes[randomNumber].status=infected;
   infect++;
   int k = IndexOf(Patient);
 for(int days = 1 ; days <= 100 ; days++){
   for(int i=0 ;i<NoV;i++){
   Edge* edge = nodes[i].edPtr;
   if(nodes[i].status==infected && simulate_prob(nodes[i].spreadP) && nodes[i].visit==false){

      while(edge!=NULL){
         if(nodes[edge->destination].status==vulnerable && nodes[edge->destination].visit==false){
             nodes[edge->destination].status=infected;
             nodes[edge->destination].visit =true;
             infect++;


   }

         if(nodes[edge->destination].status==infected && simulate_prob (nodes[edge->destination].diseaseP )) {
            nodes[edge->destination].status=sick;
            nodes[edge->destination].numDays++;
            sickk++;

}

         if( nodes[edge->destination].status==sick ){
            if(simulate_prob(nodes[edge->destination].recoveryR )){
              nodes[edge->destination].status = vulnerable;
              recov++;
              nodes[edge->destination].visit=false;
         }


           else{

               if( nodes[edge->destination].numDays> 7){
                nodes[edge->destination].status=dead;
                gone++;
                nodes[edge->destination].numDays=0;
                nodes[edge->destination].visit=true;
                sickk--;

           }
           }
         }

      edge=edge->nPtr;
}

    }
    }

        }

}
/**
 *This function is responsible for the summary of the population.
 */

void summary(){

for(int i=0 ;i<NoV ; i++){
    if(nodes[i].status==infected ){
        si++;
    }
}

for(int i=0 ; i< NoV ; i++){
    if(nodes[i].status==dead){
        if(nodes[i].age > ma){
            ma=nodes[i].age;
        }
    }
}

cout<<"Number of infected people: "<<infect<<endl;
cout<<"Number of dead people: " <<gone<<endl;
cout<<"Number of recovered people: "<<recov<<endl;
cout<<"Number of  sick people:" <<sickk-recov<<endl;
cout<<"Number of active cases:"<<si<<endl;

if(infect==0 && recov ==0){
recover_r=0;
}
else{recover_r =(recov/infect)*100;}

death_r=(gone/NoV)*100;
cout<<"Death rate:"<<death_r<<" %"<<endl;
cout<<"Recovery rate: "<<recover_r<< " %"<<endl;
if (gone ==0){
    cout<<"There are no dead people"<<endl;
}
else{
    cout<<"The oldest person to die was " <<ma<<endl;

}

}

/**
 *This function is responsible for resetting all variables and disease status back to uninfected
 */
void reset(){
     infect=0.0;
     gone=0.0;
     recov=0.0;
     sickk=0;
     si=0;
     death_r=0.0;
     recover_r=0.0;
     average_d=0;
     ma=0;

    for(int i=0 ; i<NoV ;i++){
        nodes[i].status=vulnerable;
        nodes[i].visit=false;
    }

}
/**
 *This function is responsible for reducing the spread probability of every node and runs the simulation. ( Social distancing).
 */

void policy(){
    for(int i=0; i<NoV ; i++){
        nodes[i].spreadP=nodes[i].spreadP-0.05;
    }
     srand(time(0));
   int randomNumber;

   randomNumber=(rand()%NoV);
   string Patient=nodes[randomNumber].name;
  // cout<<"patient 1:"<<Patient<<endl;
   nodes[randomNumber].status=infected;
   infect++;
   int k = IndexOf(Patient);
 for(int days = 1 ; days <= 100 ; days++){
   for(int i=0 ;i<NoV;i++){
   Edge* edge = nodes[i].edPtr;
   if(nodes[i].status==infected && simulate_prob(nodes[i].spreadP) && nodes[i].visit==false){

      while(edge!=NULL){
         if(nodes[edge->destination].status==vulnerable && nodes[edge->destination].visit==false){
             nodes[edge->destination].status=infected;
             nodes[edge->destination].visit =true;
             infect++;


   }

         if(nodes[edge->destination].status==infected && simulate_prob (nodes[edge->destination].diseaseP )) {
            nodes[edge->destination].status=sick;
            nodes[edge->destination].numDays++;
            sickk++;

}

         if( nodes[edge->destination].status==sick ){
            if(simulate_prob(nodes[edge->destination].recoveryR )){
              nodes[edge->destination].status = vulnerable;
              recov++;
              nodes[edge->destination].visit=false;
         }


           else{

               if( nodes[edge->destination].numDays> 7){
                nodes[edge->destination].status=dead;
                gone++;
                nodes[edge->destination].numDays=0;
                nodes[edge->destination].visit=true;
                sickk--;

           }
           }
         }

      edge=edge->nPtr;
}

    }
    }

        }
}

};


