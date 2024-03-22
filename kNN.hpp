
#include "main.hpp"


/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */

int partition(double* arr, int* labels, int low, int high);

void quickSelect(double* arr, int* labels, int low, int high);



template<typename T>
class List {
public:
  
    virtual ~List() = default;
    virtual void push_back(T value) = 0;
    virtual void push_front(T value) = 0;
    virtual void insert(int index, T value) = 0;
    virtual void remove(int index) = 0;
    virtual T& get(int index) const = 0;
    virtual int length() const = 0 ;
    virtual void clear() = 0;
    virtual void print() const = 0;
    virtual void reverse() = 0;

    
    // virtual List<T>* subList(int start, int end) = 0;
    virtual void printTtoE(int start, int end)=0;
    
};

template<typename T>
class Image : public List<T>
{
    
private:
    class Node{
    public:
        T pointer;
        Node* next;
    public:
        Node(T pointer, Node* next = nullptr) : pointer(pointer), next(next) {}
    };
private:
    Node* head;
    Node* tail;
    int size;
public:
    Image(){
        head = tail = nullptr;
        size = 0;
        
    }
    ~Image(){
        this->clear();
    }
    
    void push_back(T value){
        if(size==0){
            head=tail=new Node(value);
        }
        else {
            Node* temp= new Node(value);
            tail->next=temp;
            tail=temp;
            
        }
       size++;

    }
    void push_front(T value){
       
        if(size==0) {
        
            head=tail= new Node(value);;
        }
        else {
            head=new Node(value,head);
        }
       size++;

    }
    void insert(int index, T value){
        if (index < 0 || index > size) return;

    Node* newNode = new Node(value);
    if (index == 0) {
        newNode->next = head;
        head = newNode;
    } else {
        Node* temp = head;
        for (int i = 0; i < index - 1; ++i) {
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
    }

    if (index == size) { // Update tail if inserted at the end
        tail = newNode;
    }

    size++;
        }
        
    
    void remove(int index){
        if(index < 0 || index >= size) return;
        Node *deleteN;     
        if(this->size==1){
            deleteN= head;
            head=tail=nullptr;
              
        }
        else if( index==0){
            deleteN=head;
            head=head->next;
              
        }
        else {
            Node *temp=head;
            for(int i=1;i<index;i++) temp= temp->next;
            deleteN=temp->next;
            temp->next=deleteN->next;
            if(index==this->size-1) tail=temp;


        }
    delete deleteN;
    size--;


    }

    T& get(int index) const{
        if(index < 0 || index >= this->length())  throw out_of_range("get(): Out of range");
        Node *temp=head;
        for(int i=0;i<index;i++) temp=temp->next;
        return temp->pointer;
    }

    int length() const{
        return size;
    }

    void clear(){
        Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
        
    }

    void print() const{
    if(size==0) return;
    Node* temp = head;
    while (temp != nullptr) {
        cout << temp->pointer << (temp->next ? " " : "\n");
        temp = temp->next;
    }
    }

    void reverse(){  
        Node *prev = nullptr, *current = head, *next = nullptr;
        tail = head; // Tail will become the new head

    while (current != nullptr) {
        next = current->next;  // Store next
        current->next = prev;  // Reverse current node's pointer
        prev = current;        // Move pointers one position ahead
        current = next;
    }
    head = prev; // Reset head to the new first element

    }

    

    // List<T>* subList(int start, int end){
    //    if(this->size<=start) return nullptr;
    //    Node *temp= head;
    //    for(int i=0;i<start;i++){
    //     temp=temp->next;
    //    }
    //    List<T>* result = new Image<T>();
    //     for(int i=start; i  < this->size && i < end;i++){
    //         result->push_back(temp->pointer);
    //          temp=temp->next;
    //     }

    //     return result;

    // }
    void printTtoE(int start, int end){
        end=min(end,this->size-1);
        if (start < 0 || start >= this->length() || end < start)
        {
            return;
        }
        Node *pointer = head;
        for (int i = 0; i < start; i++)
        {
            pointer = pointer->next;
        }
        for (int i = start; i <= end && pointer != nullptr; i++)
        {
            cout << pointer->pointer;
            if (i != end)
            {
                cout << " ";
            }
            pointer = pointer->next;
        }
    }
   


};
template class Image<int>;
template class Image<string>;

class Dataset {
private:
    List<List<int>*>* data;
    List<string>* NameCol;
    //You may need to define more
public:
    
    Dataset();
    ~Dataset();
    
    Dataset(const Dataset& other);
    Dataset& operator=(const Dataset& other);
    bool loadFromCSV(const char* fileName);
    void getShape(int& nRows, int& nCols) const;
    void columns() const;
    List<List<int>*>* getData() const;
    
     double score(const Dataset& y_test) const
    {   
          if (this->data->length() != y_test.data->length()) return -1;

    double correct_predictions = 0;
    for (int i = 0; i < this->data->length(); i++) {
        double this_label = this->data->get(i)->get(0);
        double test_label = y_test.data->get(i)->get(0);
        if (this_label == test_label) {
            correct_predictions++;
        }
    }
    return static_cast<double>(correct_predictions) / this->data->length();
    }
    Dataset predict(const Dataset& X_train, const Dataset& Y_train, const int k) const;
    
    



void printHead(int nRows=5 , int nCols=5 ) const
    {    
    if (this->data->length() == 0 || this->NameCol->length() == 0||nRows<=0||nCols<=0) {
        return;
    }

    
   int nCols1 = min(nCols, this->data->length());
   this->NameCol->printTtoE(0,nCols1-1);
   cout << endl;
   
    // for (int col = 0; col < nCols; ++col) {
    //     if(col!=nCols-1){
    //     cout << this->NameCol->get(col) << " ";
    // }   
    // else cout << this->NameCol->get(col) << endl;

    // }
   
  int  nRows1 = min(nRows, this->data->length());
    for(int i=0;i<nRows;++i){
        this->data->get(i)->printTtoE(0,nCols1-1);
        if(i==nRows1-1) break;
        else cout << endl;
       
    }

   
    // for (int row = 0; row < nRows; ++row) {
      
    //     List<int>* currentRow = this->data->get(row);
    //     cout << currentRow->get(0);
    // if(row!=nRows-1){
    //     for (int col = 1; col < nCols; ++col) {
    //         cout << " " << currentRow->get(col);
    //     }
    //     cout << endl;
    // }
    // else{
    //      for (int col = 1; col < nCols; ++col) {
    //         cout << " " << currentRow->get(col);
    //     }
    // }
    
    // }
    
    }
        
    
void printTail(int nRows = 5, int nCols = 5) const
    {
      if (this->data->length() == 0 || this->NameCol->length() == 0 || nRows <= 0||nCols<=0) {
        return;
    }

    int TRows = this->data->length();
    nRows = nRows > TRows ? TRows : nRows;
    int TCols = this->NameCol->length();
    nCols = nCols < 0 ? TCols : min(nCols, TCols);

    // Calculate the starting index for rows to begin printing from
    nCols = min(nCols, this->NameCol->length());
    this->NameCol->printTtoE(this->NameCol->length() - nCols, this->NameCol->length() - 1);
    cout << endl;
    nRows = min(nRows, this->data->length());
    for (int j = this->data->length() - nRows; j < this->data->length(); ++j)
    {
        this->data->get(j)->printTtoE(this->data->get(j)->length() - nCols, this->data->get(j)->length() - 1);
        if (j == this->data->length() - 1) break;
        else cout << endl;
    }
 
    }
    
bool drop(int axis = 0, int index = 0, string columns = "")
    {
   
    if (axis == 0 ||(axis != 0 && axis != 1)) {
        if (index < 0 || index >= this->data->length()) {
            return false;
        }
       
        this->data->remove(index);
        return true;
    }
    if(this->data->length()==0) return false;
    
    if (axis == 1) {
        int colIndex = -1;
        for (int i = 0; i < this->NameCol->length(); ++i) {
            if (this->NameCol->get(i) == columns) {
                colIndex = i;
                break;
            }
        }
        if (colIndex == -1) {
           
            return false;
        }
      
        for (int i = 0; i < this->data->length(); ++i) {
            List<int>* row = this->data->get(i);
            row->remove(colIndex);
        }
        
        this->NameCol->remove(colIndex);
        return true;
    }
    return false;
    }
Dataset extract(int startRow = 0, int endRow = -1, int startCol = 0, int endCol = -1) const
    {
    if (  endRow < -1 || endCol < -1 || startRow > endRow && endRow != -1 || startCol > endCol && endCol != -1) {
        throw out_of_range("get(): Out of range");
    }
     Dataset extracted;
    int adjustedEndRow = (endRow == -1) ? this->data->length()-1  : std::min(endRow, this->data->length() - 1);
    int adjustedEndCol = (endCol == -1) ? this->NameCol->length() - 1 : std::min(endCol, this->NameCol->length() - 1);
   


    for (int col = startCol; col <= adjustedEndCol; ++col) {
        extracted.NameCol->push_back(this->NameCol->get(col));
    }

    // Extract the data rows
    for (int row = startRow; row <= adjustedEndRow; ++row) {
        List<int>* newRow = new Image<int>();
        for (int col = startCol; col <= adjustedEndCol; ++col) {
            newRow->push_back(this->data->get(row)->get(col));
        }
        extracted.data->push_back(newRow);
        
    }
   
    return extracted;
    }

};


class kNN {
private:
int k;
Dataset X_train;
Dataset Y_train;
 // access Dataset data form ImageClass
//You may need to define more
public:
kNN(int k = 5):k(k){};
void fit(const Dataset& x_train, const Dataset& y_train)
{
this->X_train = x_train;
this->Y_train = y_train;
}
Dataset predict(const Dataset& X_test) const;


double score(const Dataset& y_test, const Dataset& y_pred) const;



// List<List<int>*>* getDataFromDataset() const {}

};
void train_test_split(Dataset& X, Dataset& y, double test_size, 
                        Dataset& X_train, Dataset& X_test, Dataset& y_train, Dataset& y_test);

// Please add more or modify as needed




    double distanceEuclidean(const List<int>* x, const List<int>* y);