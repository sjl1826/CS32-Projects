// MyHash.h

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5): m_numItems(0), m_numBuckets(100)
    {
        if(maxLoadFactor<=0)
            m_maxLoadFactor = 0.5;
        else if(maxLoadFactor >=2)
            m_maxLoadFactor = 2.0;
        else
            m_maxLoadFactor = maxLoadFactor;
        arr = new Node*[100];
        for(int i = 0; i<100;i++)
        {
            arr[i] = nullptr;
        }
        
    }
    ~MyHash()
    {
        for(int i = 0; i<m_numBuckets; i++)
        {
            Node* bucketdelete = arr[i];
            while(bucketdelete!= nullptr)
            {
                Node* temp = bucketdelete;
                bucketdelete = bucketdelete->next;
                delete temp;
            }
        }
        delete [] arr;
    }
    void reset()
    {

        for(int i = 0; i<m_numBuckets; i++)
        {
            Node* bucketdelete = arr[i];
            while(bucketdelete!= nullptr)
            {
                Node* temp = bucketdelete;
                bucketdelete = bucketdelete->next;
                delete temp;
            }
        }
        delete [] arr;
        m_numItems = 0;
        m_maxLoadFactor = 0.5;
        m_numBuckets = 100;
        arr = new Node*[100];
        for(int j = 0; j<100;j++)
        {
            arr[j] = nullptr;
        }
    }
    
    void associate(const KeyType& key, const ValueType& value)
    {
        unsigned int hash(const KeyType&k); //prototype
        unsigned int hashed = hash(key)%m_numBuckets; //modulus it to find the bucket number
        Node* bucket = arr[hashed]; //create a temporary Node pointer to the bucket
        
        while(bucket!=nullptr) //until it reaches the end of the bucket
        {
            if(bucket->key == key)
            {
                bucket->value = value; //if it equals a key then update it and end
                return;
            }
            bucket = bucket->next; //iterate
        }
        
        double items = m_numItems+1;
        double numberofbuckets = m_numBuckets;
        double load = items/numberofbuckets; //calculate the load with one more item
        if(load>m_maxLoadFactor) //if the load with one more is greater than maxload resize
        {
            Node** temparr = new Node*[m_numBuckets*2]; //create a temporary pointer to an array of Node pointers
            for(int j = 0; j<(m_numBuckets*2);j++)
            {
                temparr[j] = nullptr;
            }
            for(int i = 0; i<m_numBuckets; i++) //loop through the old array
            {
                Node* linkedlistptr = arr[i];
                while(linkedlistptr!= nullptr)
                {
                    unsigned int temphash = hash(linkedlistptr->key)%(m_numBuckets*2); //modulus to calculate new bucket number
                    Node* placement = temparr[temphash]; //go to the bucket number
                    Node* newNode = new Node; //create the new node with the keys and values
                    Node* temp1 = linkedlistptr->next;
                    newNode->key = linkedlistptr->key;
                    newNode->value = linkedlistptr->value;
                    temparr[temphash] = newNode; //put new Node at the top of the linked list
                    newNode->next = placement; //connect the new Node to the rest of the linked list
                    linkedlistptr = temp1;
                }
            }
            for(int i = 0; i<m_numBuckets; i++)
            {
                Node* bucketdelete = arr[i];
                while(bucketdelete!= nullptr)
                {
                    Node* temp = bucketdelete;
                    bucketdelete = bucketdelete->next;
                    delete temp;
                }
            }
            delete [] arr;
            
            arr = temparr; //set the array pointer equal to the temporary one used
            m_numBuckets *= 2; //increase number of buckets
        }
        
        unsigned int hashpt2 = hash(key)%m_numBuckets;
        Node* placement = arr[hashpt2]; //go to the bucket number
        Node* newNode = new Node; //create the new node with the keys and values
        newNode->key = key;
        newNode->value = value;

        arr[hashpt2] = newNode; //put new Node at the top of the linked list
        newNode->next = placement; //connect the new Node to the rest of the linked list
        
        m_numItems++; //add to number of items
    }
    
    int getNumItems() const {return m_numItems;}
    double getLoadFactor() const { double Items = getNumItems(); double buckets = m_numBuckets; double result = Items/buckets; return result;}

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const
    {
        unsigned int hash(const KeyType&k); //prototype
        unsigned int bucketnum = hash(key)%m_numBuckets;
        if(arr[bucketnum] == nullptr)
            return nullptr;
        Node* temp = arr[bucketnum];
        while(temp!=nullptr)
        {
            if(temp->key == key)
                return &(temp->value);
            temp = temp->next;
        }
        if(temp == nullptr)
            return nullptr;
        return &(arr[bucketnum]->value);
    }

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;

private:
    double m_maxLoadFactor;
    int m_numItems;
    int m_numBuckets;
    struct Node
    {
        Node(){next = nullptr;}
        KeyType key;
        ValueType value;
        Node* next;
    };
    Node** arr;
};
