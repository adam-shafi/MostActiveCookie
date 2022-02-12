/*
* Most Active Cookie
* Written By: Adam Shafi
* Program processes a log file and outputs the most active cookie for specified day. 
*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;


/*
* Nodes used for Singely-Linked List.
* 
* Stores the next node, a cookie, and the number of times that cookie has been seen.
*/
class Node
{

private:
    /*
    * Member variables
    * Stores a pointer to the next node, a cookie string, and a cookie count
    */
    Node* next;
    std::string cookie;
    int cookieCount;
public:
    /*
    * Constructors
    * Initializes instance variables to null-ish values if no parameters.
    * Initializes instance variables to parameter's values if entered.
    */
    Node() { next = nullptr; cookie = ""; cookieCount = 0; }
    Node(std::string payload) { next = nullptr; cookie = payload; cookieCount = 1; }
    // Destructor
    ~Node() {  }

    /*
    * Accessors for next, cookie, and count
    * @params none
    * @return requested value
    */
    Node* getNext() const { return next; }
    std::string getCookie() const { return cookie; }
    int getCount() const { return cookieCount; }

    /*
    * Mutators for next, cookie, and count
    * @params the new value to be stored
    * @return none
    */
    void setCookie(std::string newCookie) { cookie = newCookie; }
    void setNext(Node* newNext) { next = newNext; }
    void setCount(int count) { cookieCount = count; }
    void incrementCount() { cookieCount++; }            //Increments cookie count


};

/*
* Singely-Linked List that stores the head and tail. 
*/
class LinkedList
{
private:
    /*
    * Member Variables
    * Stores a pointer to the head and tail nodes.
    */
    Node* head;
    Node* tail;
public:
    /*
    * Constructor
    * Initializes head and tail to point to nullptr
    */
    LinkedList() { head = nullptr; tail = nullptr; }
    /*
    * Destructor
    * Loops through the entire list and deletes every Node.
    */
    ~LinkedList() {
        if (head) {
            Node* cur = head->getNext();                    //Sets cur to head's next
            Node* prev = head;                              //Sets prev to head
                    
            while (cur && prev) {                           //While both cur and prev aren't nullptr
                delete prev;                                //Delete prev
                prev = cur;                                 //Set prev to cur
                cur = cur->getNext();                       //Set cur to cur's next
            }
            if (prev) { delete prev; }                      //If prev isn't nullptr, delete prev.
        }
    }
    /*
    * Checks if the Node to be added already exists in the list. If it does, increment its cookie count.
    * Otherwise, if the list has a head, appends the Node to the end of the list and sets it as the tail. 
    * If the list does not have a head (is empty), sets this Node as the head and tail.
    * 
    * @params the Node to be appended
    * @return none
    */
    void append(Node* payload) {
        Node* duplicate = search(payload->getCookie());     //Searches the list for a duplicate
        if (duplicate) {                                    //If the duplicate exists
            duplicate->incrementCount();                    //Increment cookie count
        }
        else {                                              //List doesn't have duplicate
            if (head) {                                     //If head isn't nullptr
                tail->setNext(payload);                     //Set tail's next to payload
            }
            else {                                          //Head is nullptr, list is empty
                head = payload;                             //Sets head to payload
            }   
            tail = payload;                                 //Sets tail to payload
        }
    }
    /*
    * Checks if the Node to be added already exists in the list. If it does, increment its cookie count.
    * Otherwise, if the list has a head, prepends the Node to the beginning of the list and sets it as the head.
    * If the list does not have a head (is empty), sets this Node as the head and tail.
    *
    * @params the Node to be prepended
    * @return none
    */
    void prepend(Node* payload) {
        Node* duplicate = search(payload->getCookie());     //Searches the list for a duplicate
        if (duplicate) {                                    //If the duplicate exists
            duplicate->incrementCount();                    //Increment cookie count
        }
        else {                                              //List doesn't have duplicate
            if (head) {                                     //If head isn't nullptr
                payload->setNext(head);                     //Set head's next to payload
            }
            else {                                          //Head is nullptr, list is empty
                tail = payload;                             //Sets tail to payload
            }
            head = payload;                                 //Sets head to payload
        }
    }
    /*
    * Returns the head
    * 
    * @params none
    * @return the head node pointer
    */
    Node* front() { return head; }
    /*
    * Removes the head node. Disconnects the removed node from the list to prevent security breaches and then returns it.
    * 
    * @params none
    * @return the head node pointer
    */
    Node* removeFront() {
        Node* removeNode = nullptr;                         //Initializes removeNode to nullptr
        if (head) {                                         //If head isn't nullptr (list isn't empty)
            removeNode = head;                              //Sets the removeNode to head
            if (head == tail) {                             //If head == tail, this is the last node in the list
                tail == nullptr;                            //Sets tail to nullptr
            }
            head = head->getNext();                         //Sets head to head's next
            removeNode->setNext(nullptr);                   //Disconnects removeNode from the list
        }
        return removeNode;                                  //Returns the removed node
    }

    /*
    * Searches the list for the specified cookie. 
    * 
    * @param the cookie to be searched for
    * @return the node that contains the cookie. nullptr if it doesn't exist.
    */
    Node* search(std::string cookieName) {
        Node* cur = head;                                   //Sets the current node to head
        while (cur) {                                       //While cur isn't nullptr
            std::string searchName = cur->getCookie();      //Gets the cookie of the current node
            if (cookieName == searchName) {                 //If the cookie of the current node is the one we're looking for
                return cur;                                 //Return that node
            }
            cur = cur->getNext();                           //Sets cur to cur's next
        }
        return nullptr;                                     //cookie wasn't found. return nullptr.
    }

};

//Function Prototypes
int readFile(ifstream&, LinkedList*, string);
void printMostActive(LinkedList*, int);

int main(int argc, char* argv[])
{
    //Ensures the proper command was called. If it wasn't, outputs an error message and exits.
    if (argc != 4) {
        cout << "Please follow the following format:\n./<program_name> <log file name> <flag> <date>\n";
        return 1;
    }
    ifstream inputFile(argv[1]);                            //Opens the file stream from the log file
    if (!inputFile) {                                       //If the file didn't open properly
        cout << "File didn't open properly\n";              //Lets the user know, then exits.
        return 1;
    }
    string date = argv[3];                                  //Gets the date
    LinkedList* list = new LinkedList();                    //Creates an empty linked list

    //Populates the list with cookies containing the date. Max count contains the highest cookie count in the list.
    int maxCount = readFile(inputFile, list, date);        
    printMostActive(list, maxCount);                        //Displays the most active cookie(s) in the list
    inputFile.close();                                      //Closes the input file
    delete list;                                            //Deletes the linked list
    return 0;                                               //Exits the program
}
/*
* Reads the file and puts all cookies with the date in the linked list. Returns the highest count of cookies.
* 
* @params the inputfile to be read, the linked list to store the cookies, the date we want to check
* @returns the maximum cookie count
*/
int readFile(ifstream& inputFile, LinkedList* list, string date) {
    string line;
    string cookie;
    int maxCount = 0;
    while (getline(inputFile, line)) {                      //While the input file isn't empty
        if (line == "") { return maxCount; }                //If the line is empty, return maxCount

        //If the date and a comma are on this line (just to double check, as it will be used)
        if (line.find(date) < line.size() && line.find(',') < line.size()) {
            cookie = line.substr(0, line.find(','));        //Extracts the cookie from the line
            Node* cookieNode = new Node(cookie);            //Creates the cookie node
            list->append(cookieNode);                       //Appends the cookie to the linked list

            //Retrives the cookie. If the added cookie's count > max count, updates max count
            Node* addedCookie = list->search(cookie);       
            if (addedCookie->getCount() > maxCount) { maxCount = addedCookie->getCount(); }
        }
    }
    return maxCount;                                        //Returns max count
}

/*
* Prints every node in the list that has a count equal to max count
* 
* @params the linked list that holds the cookie nodes, the max cookie count
* @return nothing
*/
void printMostActive(LinkedList* list, int maxCount) {
    Node* cur = list->front();                              //Sets the current node to the head
    while (cur) {                                           //While cur isn't nullptr
        if (cur->getCount() == maxCount) {                  //If cur's count == max count
            cout << cur->getCookie() << '\n';               //Output cur's cookie
        }
        cur = cur->getNext();                               //Set cur to cur's next
    }
}
