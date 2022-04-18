package main

import (
    "net/http"
    "github.com/gin-gonic/gin"
    "errors"
    "time"
    "strconv"
)

type book struct {
    ID      string  `json:"id"`
    Title   string  `json:"title"`
    Author  string  `json:"author"`
    Quantity  int   `json:"quantity"`
}

var books = []book{
    {ID: "1", Title: "Spektrum Deutsch B1", Author: "Anne Buscha", Quantity: 2},
    {ID: "2", Title: "Menschen Hier", Author: "Anna Beritsameter", Quantity: 3},
    {ID: "3", Title: "Effective Modern C++", Author: "Scott Meyers", Quantity: 6},
}

type lendInfo struct {
    ID          string      `json:"id"`
    BookID      string      `json:"bookId"`
    StudentID   string      `json:"studentId"`
    IssueTime   time.Time   `json:"issueTime"`
    ReturnTime   time.Time   `json:"returnTime"`
}

var lendInfos = []lendInfo{
    {ID: "1", BookID: "1", StudentID: "1", IssueTime: time.Now(), ReturnTime: time.Now()},
}

var lendCount = 1

func getLendInfos(c *gin.Context){
    c.IndentedJSON(http.StatusOK, lendInfos)
}

func getBooks(c *gin.Context){
    c.IndentedJSON(http.StatusOK, books)
}

// {
//   "id": "4",
//   "title": "Der Mann, der die Zahlen liebte",
//   "author": "Paul Hoffman",
//   "quantity": 7
// }

func createBook(c *gin.Context){
    var newBook book

    if err := c.BindJSON(&newBook); err != nil {
        return
    }

    books = append(books, newBook)
    c.IndentedJSON(http.StatusCreated, newBook)
}

func getBookById(id string) (*book, error){
    for i, b:= range books{
        if b.ID == id {
            return &books[i], nil
        }
    }

    return nil, errors.New("book not found")
}

func checkoutBook(c *gin.Context){
    id, ok := c.GetQuery("id")
    studentId, ok2 := c.GetQuery("student-id")

    if !ok || !ok2 {
        c.IndentedJSON(http.StatusBadRequest, gin.H{"message": "Missing id query parameter."})
        return
    }

    book, err := getBookById(id)

    if err != nil {
        c.IndentedJSON(http.StatusNotFound, gin.H{"message": "Book not found."})
        return
    }

    if book.Quantity < 1 {
        c.IndentedJSON(http.StatusBadRequest, gin.H{"message": "Book not available."})
        return
    }

    // update lend information
    var newLend lendInfo;
    newLend.ID = strconv.Itoa(lendCount + 1)
    newLend.BookID = book.ID
    newLend.StudentID = studentId
    newLend.IssueTime = time.Now()
    newLend.ReturnTime = time.Now().Add(time.Hour * 24 * 7 * time.Duration(2))
    lendInfos = append(lendInfos, newLend)

    book.Quantity -= 1
    c.IndentedJSON(http.StatusOK, book)
}

func returnBook(c *gin.Context) {
    id, ok := c.GetQuery("id")

    if !ok {
        c.IndentedJSON(http.StatusBadRequest, gin.H{"message": "Missing id query parameter."})
        return
    }

    book, err := getBookById(id)

    if err != nil {
        c.IndentedJSON(http.StatusNotFound, gin.H{"message": "Book not found."})
        return
    }

    book.Quantity += 1
    c.IndentedJSON(http.StatusOK, book)
}

func bookByID(c *gin.Context){
    id := c.Param("id") // book/2
    book, err := getBookById(id)

    if err != nil{
        c.IndentedJSON(http.StatusNotFound, gin.H{"message": "Book not found."})
        return
    }

    c.IndentedJSON(http.StatusOK, book)
}

func main(){
    router := gin.Default()

    router.GET("/books", getBooks)
    router.GET("/book/:id", bookByID)
    router.PATCH("/book/checkout", checkoutBook)
    router.PATCH("/book/return", returnBook)
    router.POST("/books", createBook)

    router.GET("/lend-infos", getLendInfos)

    router.Run("localhost:8080")
}
