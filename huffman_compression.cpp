#include<bits/stdc++.h>
using namespace std;


// structure to store hoffman code to each character
struct huffman_bits{
  int value;
  int length;
};


// node of tree
struct tree_node{
  tree_node *left;
  tree_node *right;
  char character;
  int weight;
};

// assign code to each character
// initially we start with one
// moving left append 0
// moving right append 1
void getBits(tree_node *root, huffman_bits soFar, map<char, huffman_bits> &values){
  if(root == NULL) return;
  if(root->left == NULL && root->right == NULL){
    values[root->character] = soFar;
  }
  soFar.length++;
  soFar.value <<= 1;
  getBits(root->left, soFar, values);
  soFar.value |= 1;
  getBits(root->right, soFar, values);
}



	//return the binaary representation of the code for that character in string
string bitsToString(huffman_bits bits){
  int b = bits.value;
  string retval;
  for(int i = 0; i < bits.length; i++){
    if(b & 1 != 0){
      retval = "1" + retval;
    }else{
      retval = "0" + retval;
    }
    b >>= 1;
  }
  return retval;
}




// argc is number of arguments passed
// argv is the vector storing arguments
// program name is always the first argument
int main(int argc, char **argv)
{

	if(argc < 2)
	{
    cout << "Please input the file" << endl;
    return 0;
    }


    // input stream class to operate on files
    ifstream file;

    // open the file 
    // second argument is the mode(input / output)
    file.open(argv[1], ios::in);

    // if some error occur in opening the file
    if(!file.is_open())
    {
    cout << "Unable to open file" << endl;
    return 0;
  	}
  	else cout<<"file opened successfully"<<endl;


  	// calculating Frequencies
  char current;
  map<char, int> counts;

  cout << "Counting Frequencies" << endl;

  // read while the file not end
  while(!file.eof())
  {
    current = file.get();
    // check if there is some error
    if(file.good())
        counts[current] ++;
  }

  if(counts.find('\0') == counts.end())
    counts['\0'] = 0;

  // Calculate bits
  cout << "Creating Huffman Tree" << endl;
  priority_queue <pair<int,tree_node*>, vector<pair<int,tree_node*> >, greater<pair<int,tree_node*> > > pq;

  // traversing map and creating node
  for(map<char, int>::iterator i = counts.begin(); i != counts.end(); i++){
    tree_node *node = (tree_node *)malloc(sizeof(tree_node));
    node->left = NULL;
    node->right = NULL;
    node->character = i->first;
    node->weight = i->second;
    pq.push(make_pair(node->weight,node));
  }


  while(pq.size()>1)
  {

  	pair<int,tree_node*>p1=pq.top();
  	pq.pop();
  	pair<int,tree_node*>p2=pq.top();
  	pq.pop();

  	tree_node* first,*second;

  	first=p1.second;
  	second=p2.second;



  	tree_node *newNode = (tree_node *)malloc(sizeof(tree_node));
    newNode->left = first;
    newNode->right = second;
    newNode->character = '\0';
    newNode->weight = first->weight + second->weight;

    pq.push(make_pair(newNode->weight,newNode));

  }

   map<char, huffman_bits> codes;
  huffman_bits initial;
  initial.value = 1;
  initial.length = 1;

  getBits(pq.top().second, initial, codes);

cout<<"got code for each character"<<endl;

// we now make new file to store this code

//Output stream class to operate on files

  ofstream out;
  string filename = argv[1];

  // creating file of any extension that dont exist
  filename += ".zzip";

  //The constructor for ifstream used to only take a const char * (which is what the c_str() method provides
  out.open(filename.c_str(), ios::out | ios::binary);


  //close the file as eof bit is true;
  file.close(); 
  //reopen the same input file
  file.open(argv[1], ios::in);
 

   cout << "Writing code" << endl;
  for(map<char, huffman_bits>::iterator i = codes.begin(); i != codes.end(); i++){
    if(i->first != '\0'){
      //Inserts the first n characters of the array pointed by s into the stream
      //This function simply copies a block of data, without checking its contents: The array 
      // may contain null characters, which are also copied without stopping the copying process
      out.write(&i->first, 1);
      out.write((char *)&i->second.value, 4);
    }
  }



  //we encounter '\0' when the line  ends;
  char flag = '\0';
  out.write(&flag, 1);
  out.write((char *)&codes['\0'].value, 4);
  


  int currentData = 0;
  int currentLength = 0;
  while(!file.eof()){
    current = file.get();
    if(file.good()){
      huffman_bits these = codes[current];
      // cout << "Writing character: " << current << " with bits: " << bitsToString(these) << endl;
      // cout << "These bits:" << bitsToString(these) << endl;
      if(currentLength + these.length <= sizeof(currentData) * 8){
        currentData <<= these.length;
        currentData |= these.value;
        currentLength += these.length;
      }else{
        int diff = sizeof(currentData) * 8 - currentLength;
        currentData <<= diff;
        int newData = these.value >> these.length - diff;
        currentData |= newData;
        out.write((char *)&currentData, 4);
        newData = these.value;
        int shamt = (sizeof(currentData)*8) - these.length;
        currentData = (these.value << shamt) >> shamt;
        currentLength = these.length - diff;
      }

      if(currentLength == sizeof(currentData) * 8){
        out.write((char *)&currentData, 4);
        currentLength = 0;
        currentData = 0;
      }
    }
  }


  if(currentLength > 0){
    int shamt = (sizeof(currentData)*8) - currentLength;
    currentData <<= shamt;
    out.write((char *)&currentData, 4);
  }

}