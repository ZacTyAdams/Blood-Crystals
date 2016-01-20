/// \file AbstractList.h
/// \brief The templated class CAbstractList.

#pragma once

/// \brief The abstract list.

template <class thing>
class CAbstractList{
  protected:
    thing** m_pList; ///< List of things.
    int m_nCount; ///< How many things in list.
    int m_nSize; ///< Maximum size of list.
    
  public:
    CAbstractList(int size); ///< Constructor
    ~CAbstractList(); ///< Destructor
    bool Insert(thing* newthing); ///< Insert new thing.
    thing* create(); ///< Create and insert new thing.
    void clear(); ///< Clear the list.
}; //CAbstractList

//WARNING:
//The member function code actually has to go in the header file, not
//a separate code file. This is because it isn't really code, it's a 
//declaration of how the code will look once the template is instantiated.
//So don't try to move it out into a separate cpp file. That way madness lies.

template <class thing>
CAbstractList<thing>::CAbstractList(int size){ //constructor
  m_pList = new thing* [size];
  m_nCount = 0; //no things
  for(int i=0; i<size; i++) //null our object list
    m_pList[i] = nullptr;
  m_nSize = size; //this is the size
} //constructor

template <class thing>
CAbstractList<thing>::~CAbstractList(){ //destructor
  for(int i=0; i<m_nSize; i++) //for each thing
    delete m_pList[i]; //delete it
  delete [] m_pList;
} //destructor

template <class thing>
bool CAbstractList<thing>::Insert(thing* newthing){
  if(m_nCount < m_nSize){ //if room
    int i=0; while(m_pList[i])i++; //find first free slot
    m_pList[i] = newthing;
    m_nCount++; //one more damn thing
    return true;
  } //if
  else return false;
} //Insert

template <class thing>
thing* CAbstractList<thing>::create(){
  thing* p = new thing();
  if(Insert(p)) //succeed
    return p;
  else{ //fail
    delete p; 
    return nullptr;
  } //else
} //create

template <class thing>
void CAbstractList<thing>::clear(){ 
  m_nCount = 0; //no objects
  for(int i=0; i<m_nSize; i++){ //for each thing
    delete m_pList[i]; //delete it
    m_pList[i] = nullptr; //safe delete
  } //for
} //Clear