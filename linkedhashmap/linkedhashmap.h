#define TAM_ARRAY 1000000

typedef enum { false, true } bool;

typedef struct _Element {
  int key;
  char* value;
  struct _Element *next;
  struct _Element *prev;
} Element;

typedef struct _Node {
  Element *element;
  struct _Node *next;
} Node;

typedef struct _Bucket {
  Node *first;
  Node *last;
} Bucket;

typedef struct _LinkedHashMap {
  Bucket **buckets;
  Element *mostRecent;
  Element *lessUsed;

  int maxSize;
  int hashSize;
  int size;
} LinkedHashMap;

void printCache(LinkedHashMap* linkedHashMap);
void printLinkedHashMap(LinkedHashMap* linkedHashMap);
LinkedHashMap* newLinkedHashMap(int maxSize, int hashSize);
Node* newNode(Element *element);
Element* newElement(int key, char* value);
void addNode(Bucket* bucket, Node* node);
void freeElement(Element* element);
void freeNode(Node* node);
void deleteNode(LinkedHashMap* linkedHashMap, Bucket* bucket, int key);
void addElement(LinkedHashMap* linkedHashMap, int key, char* value);
void deleteElement(LinkedHashMap* linkedHashMap, int key);
bool elementExists(LinkedHashMap* linkedHashMap, int key);
char* getElement(LinkedHashMap* linkedHashMap, int key);

void addElementOnCache(LinkedHashMap* linkedHashMap, Element* element);
void updateElementPositionOnCache(LinkedHashMap* linkedHashMap, Element* element);
void removeElementFromCache(LinkedHashMap* linkedHashMap, Element* element);
