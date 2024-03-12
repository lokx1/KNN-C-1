#include "kNN.hpp"


//**********************************************************************
//* class Image thừa kế từ List<T>
//**********************************************************************





//**********************************************************************
//* class Dataset
//**********************************************************************

Dataset::Dataset()
{
  //TODO: implement task 2
    this->NameCol= new Image<string> ();
    this->data= new Image<List<int>*>();
}
Dataset::~Dataset()
{  
    for (int i=0;i<data->length();i++){
        data->get(i)->clear();

    }
   
    for(int i = 0; i< data->length(); i++){
        delete this->data->get(i);
    }
   
    delete data;
    delete NameCol;
}
Dataset::Dataset(const Dataset& other)
{
     this->NameCol = new Image<string>();


    for (int i = 0; i < other.NameCol->length(); ++i) {
    this->NameCol->push_back(other.NameCol->get(i));
}

    
    this->data = new Image<List<int>*>();

  
    for (int i = 0; i < other.data->length(); ++i) {
        List<int>* originalList = other.data->get(i);
        List<int>* copiedList = new Image<int>();

      
        for (int j = 0; j < originalList->length(); ++j) {
            copiedList->push_back(originalList->get(j));
        }

        
        this->data->push_back(copiedList);
    }

}
Dataset& Dataset::operator=(const Dataset& other)
{
        if (this != &other) { // Protect against self-assignment
        // Clear current contents
        delete this->data;
        delete this->NameCol;

        // Deep copy of NameCol
        this->NameCol = new Image<string>();
        for (int i = 0; i < other.NameCol->length(); ++i) {
            this->NameCol->push_back(other.NameCol->get(i));
        }

        // Deep copy of data
        this->data = new Image<List<int>*>();
        for (int i = 0; i < other.data->length(); ++i) {
            List<int>* originalList = other.data->get(i);
            List<int>* copiedList = new Image<int>();
            for (int j = 0; j < originalList->length(); ++j) {
                copiedList->push_back(originalList->get(j));
            }
            this->data->push_back(copiedList);
        }
    }

    return *this;

}
bool Dataset::loadFromCSV(const char* fileName)
{   
      ifstream data1(fileName);
      if(data1.is_open()){
      string chuoi;
      int num;
      data1 >> chuoi;
      for(int i =0;i<chuoi.length();i++){
            if(chuoi[i]==',') chuoi[i]=' ';
      }
      stringstream ss(chuoi);
      while(ss>>chuoi) NameCol->push_back(chuoi);
      while(data1>>chuoi) {
        for(int i=0;i<chuoi.length();i++){
            if(chuoi[i]==',') chuoi[i]=' ';
        }
    stringstream ss(chuoi);
    List<int>* temp = new Image<int>();
    while(ss>>num) temp->push_back(num);
     data->push_back(temp);
      }
    return true;
      }
      return false;
}
void Dataset::getShape(int& nRows, int& nCols) const {
   nRows = this->data->length();
    nCols = (nRows > 0) ? this->data->get(0)->length() : 0;
}
void Dataset::columns() const
{  this->NameCol->print();
}

List<List<int> *> *Dataset::getData() const
{ return data;
    
}

//**********************************************************************
//* Class kNN
//**********************************************************************


void kNN::fit(const Dataset& X_train, const Dataset& y_train)
{
    this->X_train = X_train;
    this->Y_train = y_train;
}

Dataset kNN::predict(const Dataset& X_test)
{
     return X_test.predict(this->X_train,this->Y_train,this->k);
}

double kNN::score(const Dataset& y_test, const Dataset& y_pred)
{
    return y_test.score(y_pred);
}

int partition(double *arr, int *labels, int low, int high)
{
    double pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
            std::swap(labels[i], labels[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    std::swap(labels[i + 1], labels[high]);
    return (i + 1);
}

void quickSelect(double* arr, int* labels, int low, int high, int k) {
    if (low < high) {
        int pi = partition(arr, labels, low, high);

        if (pi > k) quickSelect(arr, labels, low, pi - 1, k);
        if (pi < k) quickSelect(arr, labels, pi + 1, high, k);
    }
}


//**********************************************************************
//* hàm train_test_split
//**********************************************************************
void train_test_split(Dataset& X, Dataset& Y, double test_size, 
                        Dataset& X_train, Dataset& X_test, 
                        Dataset& Y_train, Dataset& Y_test)
{
  if (test_size >= 1 || test_size <= 0)
    {
        return;
    }

    //* phân chia phần train
    int xRows, xCols, yRows, yCols;
    X.getShape(xRows, xCols);
    Y.getShape(yRows, yCols);

    X_train = X.extract(0, (1 - test_size) * xRows-1, 0, -1);

    Y_train = Y.extract(0, (1 - test_size) * yRows-1, 0, 0);
   
    //* phân chia phần test

    X_test = X.extract((1 - test_size) * xRows , xRows, 0, -1);

    Y_test = Y.extract((1 - test_size) * yRows , yRows, 0, 0);
  
}


double Dataset:: distanceEuclidean(const List<int>* x, const List<int>* y) const{
        //TODO: implement Task 2 copy code từ implement Task 1 chỉnh
     double sum = 0.0;
    int length = std::min(x->length(), y->length()); // Use the smaller length if they differ, but ideally, they should be the same

    for (int i = 0; i < length; ++i) {
        double diff = x->get(i) - y->get(i);
        sum += diff * diff;
    }

    return std::sqrt(sum);



    }