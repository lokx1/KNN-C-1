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
    this->data = new Image<List<int>*>();
     this->NameCol = new Image<string>();


    for (int i = 0; i < other.NameCol->length(); ++i) {
    this->NameCol->push_back(other.NameCol->get(i));
}


  
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
      std::ifstream data1(fileName);
    if (!data1.is_open()) {
        return false;
    }

    std::string line;

    // Đọc và xử lý dòng đầu tiên chứa tiêu đề cột
    if (std::getline(data1, line)) {
        std::stringstream ss(line);
        std::string item;
        while (std::getline(ss, item, ',')) {
            NameCol->push_back(item);
        }
    }

    // Đọc và xử lý các dòng tiếp theo chứa dữ liệu
    while (std::getline(data1, line)) {
        std::stringstream ss(line);
        std::string item;
        List<int>* temp = new Image<int>();
        while (std::getline(ss, item, ',')) {
            try {
                int num = std::stoi(item); // Chuyển đổi string sang int
                temp->push_back(num);
            } catch (const std::exception& e) {
                std::cerr << "Lỗi chuyển đổi dữ liệu: " << e.what() << std::endl;
                // Xử lý phù hợp hoặc bỏ qua giá trị không hợp lệ
            }
        }
        data->push_back(temp);
    }

    return true;
}
void Dataset::getShape(int& nRows, int& nCols) const {

   nRows = this->data->length();
    nCols = (nRows > 0) ? this->data->get(0)->length() : 0;
  
}
void Dataset::columns() const
{  this->NameCol->print();
}

List<List<int> *> *Dataset::getData() const
{ return this->data;
    
}

//**********************************************************************
//* Class kNN
//**********************************************************************




Dataset kNN::predict(const Dataset& X_test) const
{
     return X_test.predict(X_train,Y_train,k);
}

double kNN::score(const Dataset& y_test, const Dataset& y_pred) const
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

void quickSelect(double* arr, int* labels, int low, int high) {
    if (low < high) {
        int pi = partition(arr, labels, low, high);

        quickSelect(arr, labels, low, pi - 1);
         quickSelect(arr, labels, pi + 1, high);
    }
}


//**********************************************************************
//* hàm train_test_split
//**********************************************************************
void train_test_split(Dataset& X, Dataset& Y, double testsize, 
                        Dataset& X_train, Dataset& X_test, 
                        Dataset& Y_train, Dataset& Y_test)
{
  if (X.getData()->length() != Y.getData()->length() || testsize >= 1 || testsize <= 0)
return;

double min = 1.0e-15;
int nrow = X.getData()->length();
double rt = nrow * (1 - testsize);

if (abs(round(rt) - rt) < min * nrow)
rt = round(rt);

X_train = X.extract(0, rt - 1, 0, -1);
Y_train = Y.extract(0, rt - 1, 0, -1);

X_test = X.extract(rt, -1, 0, -1);
Y_test = Y.extract(rt, -1, 0, -1);

}

Dataset Dataset::predict(const Dataset &X_train, const Dataset &Y_train, const int k) const
{
      Dataset result;
    result.NameCol = new Image<string>();
    result.NameCol->push_back("label");
    result.data = new Image<List<int>*>();
    
    for (int i = 0; i < this->data->length(); ++i) {
        List<int>* predictedLabelList = new Image<int>();
        double* distances = new double[X_train.data->length()];
        int* indices = new int[X_train.data->length()];

        for (int j = 0; j < X_train.data->length(); ++j) {
            distances[j] = distanceEuclidean(this->data->get(i), X_train.data->get(j));
            indices[j] = j;
        }

       

       quickSelect(distances,indices,0,X_train.data->length()-1);
       
        int labelCounts[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        for (int i = 0; i < k; i++)
        {   
            int label = Y_train.data->get(indices[i])->get(0);
            labelCounts[label]++;
        }
         

        int mostFrequentLabel = 0, maxCount = 0;
        for (int idx = 0; idx < 10; ++idx) {
            if (labelCounts[idx] > maxCount || (labelCounts[idx] == maxCount && idx < mostFrequentLabel)) {
                mostFrequentLabel = idx;
                maxCount = labelCounts[idx];
            }
        }

        predictedLabelList->push_back(mostFrequentLabel);
        result.data->push_back(predictedLabelList);
        delete []distances;
        delete []indices;
    }

    return result;
}


double distanceEuclidean(const List<int> *x, const List<int> *y) 
{

    if (x->length() != y->length())
    {
        return 0;
    }
    double kc = 0;
    for (int i = 0; i < x->length(); i++)
    {
        kc += pow(x->get(i) - y->get(i), 2);
    }
    return sqrt(kc);
}


