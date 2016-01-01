#include "linkedhashmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

LinkedHashMap* newLinkedHashMap(int maxSize, int hashSize) {
  LinkedHashMap* linkedHashMap = (LinkedHashMap*) malloc(sizeof(LinkedHashMap));
  linkedHashMap->maxSize = maxSize;
  linkedHashMap->size = 0;
  linkedHashMap->hashSize = hashSize;
  linkedHashMap->buckets = (Bucket**) malloc(hashSize * sizeof(Node));

  for (int i = 0; i < hashSize; i++) {
    linkedHashMap->buckets[i] = (Bucket*) malloc(sizeof(Node));
    linkedHashMap->buckets[i]->first = NULL;
    linkedHashMap->buckets[i]->last = NULL;
  }

  linkedHashMap->mostRecent = NULL;
  linkedHashMap->lessUsed = NULL;
  return linkedHashMap;
}

Node* newNode(Element *element) {
  Node *node = (Node*) malloc(sizeof(Node));
  node->element = element;
  node->next = NULL;
  return node;
}

Element* newElement(int key, char* value) {
  Element *element = (Element*) malloc(sizeof(Element));
  element->key = key;
  element->value = value;
  element->next = NULL;
  element->prev = NULL;
  return element;
}

void addNode(Bucket* bucket, Node* node) {
  if (bucket->first == NULL)
	{
		bucket->first = node;
		bucket->last = node;
	}
	else
	{
		bucket->last->next = node;
		bucket->last = node;
	}
  node->next = NULL;
}

void deleteNode(LinkedHashMap* linkedHashMap, Bucket* bucket, int key) {
  if (bucket->first->element->key == key) {
    if(bucket->first->next == NULL) bucket->last = NULL;
    Node* nextNode = bucket->first->next;

    removeElementFromCache(linkedHashMap, bucket->first->element);
    freeNode(bucket->first);

    bucket->first = NULL;
    bucket->first = nextNode;
    return;
  }

  Node* currentNode = bucket->first;
  Node* nextNode = (Node*) currentNode->next;
  while(nextNode->element->key != key) {
    currentNode = nextNode;
    nextNode = (Node*) nextNode->next;
  }
  currentNode->next = nextNode->next;

  removeElementFromCache(linkedHashMap, nextNode->element);
  freeNode(nextNode);
}

void addElement(LinkedHashMap* linkedHashMap, int key, char* value) {
  int hash = key % linkedHashMap->hashSize;
  if(!elementExists(linkedHashMap, key)) {
    //printCache(linkedHashMap);
    //printf("VOU DELETAR O %d", linkedHashMap->lessUsed->key);
    if(linkedHashMap->size >= linkedHashMap->maxSize) deleteElement(linkedHashMap, linkedHashMap->lessUsed->key);

    Element *element = newElement(key, value);
    addElementOnCache(linkedHashMap, element);
    addNode(linkedHashMap->buckets[hash], newNode(element));
    linkedHashMap->size++;
  }
  else printf("WARN: element with key %d already exisits and you tried to add it\n", key);
}

void deleteElement(LinkedHashMap* linkedHashMap, int key) {
  int hash = key % linkedHashMap->hashSize;

  if(elementExists(linkedHashMap, key)) {
    deleteNode(linkedHashMap, linkedHashMap->buckets[hash], key);
    linkedHashMap->size--;
    return;
  }
  else printf("WARN: element with key %d don't exisits and you tried to delete it\n", key);
}

bool elementExists(LinkedHashMap* linkedHashMap, int key) {
  bool exists = false;
  int hash = key % linkedHashMap->hashSize;

  Node *currentNode = linkedHashMap->buckets[hash]->first;
  while (currentNode != NULL) {
    if(currentNode->element->key == key) return true;
    currentNode = (Node*) currentNode->next;
  }

  return exists;
}

char* getElement(LinkedHashMap* linkedHashMap, int key) {
  int hash = key % linkedHashMap->hashSize;

  Node *currentNode = linkedHashMap->buckets[hash]->first;
  if(currentNode == NULL) return NULL;
  while (currentNode->element->key != key) {
    currentNode = (Node*) currentNode->next;
    if(currentNode == NULL) return NULL;
  }
  updateElementPositionOnCache(linkedHashMap, currentNode->element);
  return currentNode->element->value;
}

void addElementOnCache(LinkedHashMap* linkedHashMap, Element* element) {
  if (linkedHashMap->mostRecent == NULL)
	{
		linkedHashMap->mostRecent = element;
		linkedHashMap->lessUsed = element;
	}
	else
	{
		linkedHashMap->mostRecent->prev = element;
    element->next = linkedHashMap->mostRecent;
    linkedHashMap->mostRecent = element;
	}
}

void removeElementFromCache(LinkedHashMap* linkedHashMap, Element* element) {
  if (linkedHashMap->mostRecent->key == element->key) {
    if(linkedHashMap->mostRecent->next == NULL) linkedHashMap->lessUsed = NULL;
    Element* nextElement = linkedHashMap->mostRecent->next;
    linkedHashMap->mostRecent = nextElement;
    linkedHashMap->mostRecent->prev = NULL;
    return;
  }

  if(linkedHashMap->lessUsed->key == element->key) linkedHashMap->lessUsed = element->prev;
  element->prev->next = element->next;
  if(element->next != NULL) element->next->prev = element->prev;
}

void updateElementPositionOnCache(LinkedHashMap* linkedHashMap, Element* element) {
  if (linkedHashMap->mostRecent->key == element->key) return;

  element->prev->next = element->next;
  if(element->next != NULL) element->next->prev = element->prev;

  linkedHashMap->mostRecent->prev = element;
  element->next = linkedHashMap->mostRecent;
  linkedHashMap->mostRecent = element;
}

void freeElement(Element* element) {
  element->next = NULL;
  element->prev = NULL;
}

void freeNode(Node* node) {
  freeElement(node->element);
  node->element = NULL;
  node->next = NULL;
}

void printCache(LinkedHashMap* linkedHashMap) {
  printf("\n");

  if (linkedHashMap->mostRecent != NULL)
    printf("MostRecent: (%d, %s)\n", linkedHashMap->mostRecent->key, linkedHashMap->mostRecent->value);
  else
    printf("MostRecent: NULL\n");

  if (linkedHashMap->lessUsed != NULL)
    printf("LessUsed: (%d, %s)\n", linkedHashMap->lessUsed->key, linkedHashMap->lessUsed->value);
  else
    printf("LessUsed: NULL\n");

  Element* current = linkedHashMap->mostRecent;
  int cacheSize = 0;
  while(current != NULL) {
    cacheSize++;
    printf("(%d, %s) -> ", current->key, current->value);
    current = current->next;
  }
  printf("\n\n");
}

void printLinkedHashMap(LinkedHashMap* linkedHashMap) {
  printf("\nPrinting hashmap\n");

  for(int i = 0; i < linkedHashMap->hashSize; i++) {
    Node* current = linkedHashMap->buckets[i]->first;
    printf("Bucket %d:", i);
    while(current != NULL) {
      printf(" -> (%d, %s)", current->element->key, current->element->value);
      current = current->next;
    }
    printf(" ->\n");
  }

  printf("\n\n");
}

int main()
{
  LinkedHashMap *linkedHashMap = newLinkedHashMap(2, 2);
  printLinkedHashMap(linkedHashMap);
  printf("tamanho: %d", linkedHashMap->size);
  printCache(linkedHashMap);

  printf("\n------------------------------------ adicionando 0\n");
  addElement(linkedHashMap, 0, "valor do zero");

  printf("tamanho: %d", linkedHashMap->size);
  printCache(linkedHashMap);
  printLinkedHashMap(linkedHashMap);

  printf("\n------------------------------------ adicionando 1\n");
  addElement(linkedHashMap, 1, "valor do um");

  printf("tamanho: %d", linkedHashMap->size);
  printCache(linkedHashMap);
  printLinkedHashMap(linkedHashMap);

  printf("\n------------------------------------ adicionando 2\n");

  addElement(linkedHashMap, 2, "valor do dois");

  printf("tamanho: %d", linkedHashMap->size);
  printCache(linkedHashMap);
  printLinkedHashMap(linkedHashMap);

  printf("\n------------------------------------ adicionando 20\n");
  addElement(linkedHashMap, 20, "valor do vinte");
  printf("tamanho: %d", linkedHashMap->size);
  printCache(linkedHashMap);
  printLinkedHashMap(linkedHashMap);

  printf("\n------------------------------------ deletando 0\n");
  deleteElement(linkedHashMap, 0);

  printf("\n------------------------------------ vendo 20\n");
  printf("key 20 = %s\n", getElement(linkedHashMap, 20));
  printCache(linkedHashMap);
  printLinkedHashMap(linkedHashMap);

  printf("\n------------------------------------ vendo 2\n");
  printf("key 2 = %s\n", getElement(linkedHashMap, 2));
  printCache(linkedHashMap);
  printLinkedHashMap(linkedHashMap);

  return 0;
}
