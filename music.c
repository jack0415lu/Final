#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_VERTICES 200

typedef struct node {
    int vertex;            // 頂點索引
    float weight;          // 邊的權重，例如評分
    struct node* next;
} Node;

typedef struct {
    Node* head;            // List頭節點
} AdjacencyList;

typedef struct {
    char name[100];        // 頂點名稱，可以是用戶名或音樂名
    AdjacencyList list;    // 每個頂點
} Vertex;

typedef struct {
    Vertex vertices[MAX_VERTICES]; // 所有頂點
    int numVertices;               // 頂點數量
} Graph;

void initGraph(Graph* g) {
    g->numVertices = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        g->vertices[i].list.head = NULL;
    }
}

void addVertex(Graph* g, const char* name) {
    if (g->numVertices == MAX_VERTICES) {
        printf("Graph full\n");
        return;
    }
    strcpy(g->vertices[g->numVertices].name, name);
    g->vertices[g->numVertices].list.head = NULL;
    g->numVertices++;
}

void addEdge(Graph* g, int from, int to, float weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = to;
    newNode->weight = weight;
    newNode->next = g->vertices[from].list.head;
    g->vertices[from].list.head = newNode;
}

// Calculate similarity between two users using Euclidean distance
float calculateSimilarity(Graph* g, int user1Index, int user2Index) {
    float sum = 0;
    Node* temp1 = g->vertices[user1Index].list.head;
    Node* temp2;

    while (temp1 != NULL) {
        temp2 = g->vertices[user2Index].list.head;
        while (temp2 != NULL) {
            if (temp1->vertex == temp2->vertex) { // Same song
                sum += pow(temp1->weight - temp2->weight, 2);
            }
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }

    return 1 / (1 + sqrt(sum)); // Higher similarity means lower distance
}

void recommendMusic(Graph* g, int userIndex) {
        printf("\n");
    float maxSimilarity = 0;
    int mostSimilarUser = -1;
    
    // Find the most similar user
    for (int i = 0; i < g->numVertices; i++) {
        if (i != userIndex && g->vertices[i].list.head != NULL) { // Ensure it is a user
            float similarity = calculateSimilarity(g, userIndex, i);
            if (similarity > maxSimilarity) {
                maxSimilarity = similarity;
                mostSimilarUser = i;
            }
        }
    }
    // Add an edge to represent similarity
  
   // Recommend songs from the most similar user
if (mostSimilarUser != -1) {
    printf("Recommendations for %s based on similarity with %s:\n", g->vertices[userIndex].name, g->vertices[mostSimilarUser].name);
    addEdge(g, userIndex, mostSimilarUser, maxSimilarity);  // Add an edge representing the similarity score
        printf("Edge added between %s and %s with weight %f (similarity)\n", g->vertices[userIndex].name, g->vertices[mostSimilarUser].name, maxSimilarity);
    // Create an array to keep track of songs already rated by the original user
    int alreadyRated[MAX_VERTICES] = {0};  // Initialize all elements to 0
    Node* temp = g->vertices[userIndex].list.head;
    while (temp != NULL) {
        alreadyRated[temp->vertex] = 1;  // Mark song as already rated
        temp = temp->next;
    }

    // Find the top two highest rated songs by the most similar user that haven't been rated by the original user
    Node* candidateSongs[MAX_VERTICES] = {NULL};
    int numCandidates = 0;
    temp = g->vertices[mostSimilarUser].list.head;
    while (temp != NULL) {
        if (!alreadyRated[temp->vertex]) {  // Check if the song has not been rated by the original user
            candidateSongs[numCandidates++] = temp;  // Add to candidates if not already rated
        }
        temp = temp->next;
    }

    // Sort the candidate songs by rating in descending order
    for (int i = 0; i < numCandidates - 1; i++) {
        for (int j = i + 1; j < numCandidates; j++) {
            if (candidateSongs[j]->weight > candidateSongs[i]->weight) {
                Node* tempNode = candidateSongs[i];
                candidateSongs[i] = candidateSongs[j];
                candidateSongs[j] = tempNode;
            }
        }
    }

    // Print the top two songs if available
    int count = 0;
    for (int i = 0; i < numCandidates && count < 2; i++) {
        printf("%s\n", g->vertices[candidateSongs[i]->vertex].name);  // Print recommended song
        count++;
    }
}

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 200



int main() {
    Graph g;
    initGraph(&g);
    addVertex(&g, "User0");
    addVertex(&g, "User1");
    addVertex(&g, "User2");
    addVertex(&g, "User3");
    addVertex(&g, "Song0"); // index 4
    addVertex(&g, "Song1"); // index 5
    addVertex(&g, "Song2"); // index 6
    addVertex(&g, "Song3"); // index 7
    addVertex(&g, "Song4"); // index 8

    // Test Data Set 3
    addEdge(&g, 0, 4, 3.0);  // User0 rates Song0 3.0
    addEdge(&g, 0, 6, 2.0);  // User0 rates Song2 2.0
    addEdge(&g, 0, 7, 4.5);  // User0 rates Song3 4.5
    addEdge(&g, 1, 4, 4.0);  // User1 rates Song0 4.0
    addEdge(&g, 1, 5, 3.5);  // User1 rates Song1 3.5
    addEdge(&g, 1, 6, 4.5);  // User1 rates Song2 4.5
    addEdge(&g, 2, 5, 5.0);  // User2 rates Song1 5.0
    addEdge(&g, 2, 6, 3.5);  // User2 rates Song2 3.5
    addEdge(&g, 2, 7, 2.0);  // User2 rates Song3 2.0
    
    recommendMusic(&g, 0); // Generate recommendations for User0

    recommendMusic(&g, 2); // Generate recommendations for User2
    return 0;
}

