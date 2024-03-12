
#include "main.hpp"


/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */
int partition(double* arr, int* labels, int low, int high);

void quickSelect(double* arr, int* labels, int low, int high, int k);


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

    
    virtual List<T>* subList(int start, int end) = 0;
    
    
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
        this->size++;

    }
    void push_front(T value){
       
        if(size==0) {
        
            head=tail= new Node(value);;
        }
        else {
            head=new Node(value,head);
        }
        this->size++;

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
    this->size--;


    }

    T& get(int index) const{
        if(index < 0 || index >= this->size)  throw out_of_range("get(): Out of range");
        Node *temp=head;
        for(int i=0;i<index;i++)temp=temp->next;
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

    

    List<T>* subList(int start, int end){
       if(this->size<=start) return nullptr;
       Node *temp= head;
       for(int i=0;i<start;i++){
        temp=temp->next;
       }
       List<T>* result = new Image<T>();
        for(int i=start; i  < this->size && i < end;i++){
            result->push_back(temp->pointer);
             temp=temp->next;
        }

        return result;

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

    int correct_predictions = 0;
    for (int i = 0; i < this->data->length(); i++) {
        int this_label = this->data->get(i)->get(0);
        int test_label = y_test.data->get(i)->get(0);
        if (this_label == test_label) {
            correct_predictions++;
        }
    }
    return static_cast<double>(correct_predictions) / this->data->length();
    }
     Dataset predict(const Dataset& X_train, const Dataset& Y_train, const int k) const
    {
       Dataset result;
result.NameCol = new Image<string>();
// Giả sử NameCol của Y_train có dữ liệu, bạn cần copy cấu trúc này cho result
result.NameCol->push_back("label");
result.data = new Image<List<int>*>();
    for (int i = 0; i < this->data->length(); ++i) {
        List<int>* predictedLabelList = new Image<int>();
    // Khởi tạo mảng để lưu khoảng cách và nhãn
    double* distances = new double[X_train.data->length()];
    int* indices = new int[X_train.data->length()];

    // Tính khoảng cách
    for (int j = 0; j < X_train.data->length(); ++j) {
        distances[j] = this->distanceEuclidean(this->data->get(i), X_train.data->get(j));
        indices[j] = j; // Lưu chỉ số để sau này có thể truy xuất nhãn
    }
    
   quickSelect(distances, indices, 0, X_train.data->length() - 1, k-1);     
   
      int labelCounts[10] = {0,0,0,0,0,0,0,0,0,0};
    for (int idx = 0; idx < k; ++idx) {
    int label = Y_train.data->get(indices[idx])->get(0);
    labelCounts[label]++;
    }
    

int mostFrequentLabel = 0, maxCount = 0;
for (int idx = 0; idx < 10; ++idx) {
    // Nếu tìm thấy nhãn với số lần xuất hiện cao hơn, hoặc bằng nhưng nhãn có giá trị nhỏ hơn
    if (labelCounts[idx] > maxCount || (labelCounts[idx] == maxCount && idx < mostFrequentLabel)) {
        mostFrequentLabel = idx;
        maxCount = labelCounts[idx];
    }

}
    
   
    // Giả sử Image<int> là subclass của List<int>
    predictedLabelList->push_back(mostFrequentLabel);
 
    result.data->push_back(predictedLabelList);   
  
    delete[] distances;
    delete[] indices;
}
    return result;
}
    
    
double distanceEuclidean(const List<int>* x, const List<int>* y) const;


void printHead(int nRows=5 , int nCols=5 ) const
    {    
    if (this->data->length() == 0 || this->NameCol->length() == 0||nRows<=0||nCols<=0) {
        
        return;
    }

    
    nCols = (nCols < 0) ? this->NameCol->length() : min(nCols, static_cast<int>(this->NameCol->length()));

   
    for (int col = 0; col < nCols; ++col) {
        if(col!=nCols-1){
        cout << this->NameCol->get(col) << " ";
    }   
    else cout << this->NameCol->get(col) << endl;

    }
    

   
    nRows = min(nRows, static_cast<int>(this->data->length()));

   
    for (int row = 0; row < nRows; ++row) {
      
        List<int>* currentRow = this->data->get(row);

       
        cout << currentRow->get(0);
    if(row!=nRows-1){
        for (int col = 1; col < nCols; ++col) {
            cout << " " << currentRow->get(col);
        }
        cout << endl;
    }
    else{
         for (int col = 1; col < nCols; ++col) {
            cout << " " << currentRow->get(col);
        }
    }
    
    }
    
    }
        
    
void printTail(int nRows = 5, int nCols = 5) const
    {
      if (this->data->length() == 0 || this->NameCol->length() == 0 || nRows <= 0||nCols<=0) {
        return;
    }

    int totalRows = this->data->length();
    nRows = nRows > totalRows ? totalRows : nRows;
    int totalCols = this->NameCol->length();
    nCols = nCols < 0 ? totalCols : min(nCols, totalCols);

    // Calculate the starting index for rows to begin printing from
    
     for (int col = totalCols - nCols; col < totalCols; ++col) {
        
        if(col!=totalCols-1){
        cout << this->NameCol->get(col) << " ";
    }   
    else cout << this->NameCol->get(col);
    }
    cout << endl;
    // Print the last nRows of data
    int startRow = totalRows - nRows;
    for (int row = startRow; row < totalRows; ++row) {
        List<int>* currentRow = this->data->get(row);
        int startCol = max(0, currentRow->length() - nCols); // Calculate the starting column index
       if(row!= totalRows-1){
        for (int col = startCol; col < currentRow->length(); ++col) {
            if (col > startCol) cout << " ";
            cout << currentRow->get(col);
        }
        cout << endl;
       } else {
        for (int col = startCol; col < currentRow->length(); ++col) {
            if (col > startCol) cout << " ";
            cout << currentRow->get(col);
        }


       }
   
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

     Dataset extracted;
    int adjustedEndRow = (endRow == -1) ? this->data->length()-1  : endRow;
    int adjustedEndCol = (endCol == -1) ? this->NameCol->length() - 1 : endCol;
   

   adjustedEndRow = std::min(adjustedEndRow, this->data->length() - 1);
   adjustedEndCol = std::min(adjustedEndCol, this->NameCol->length() - 1);

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
    //You may need to define more
public:
    kNN(int k = 5):k(5){};
    void fit(const Dataset& X_train, const Dataset& y_train);
    Dataset predict(const Dataset& X_test);
   
     double score(const Dataset& y_test, const Dataset& y_pred);
};

void train_test_split(Dataset& X, Dataset& y, double test_size, 
                        Dataset& X_train, Dataset& X_test, Dataset& y_train, Dataset& y_test);

// Please add more or modify as needed
