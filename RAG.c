#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PROCESS 100
#define MAX_RESOURCE 100

typedef struct {
    char name[10];
} Process;

typedef struct {
    char name[10];
} Resource;

typedef struct {
    Process processes[MAX_PROCESS];
    Resource resources[MAX_RESOURCE];
    int graph[MAX_PROCESS][MAX_RESOURCE];
    int processCount;
    int resourceCount;
} ResourceAllocationGraph;

void initRAG(ResourceAllocationGraph* rag) {
    rag->processCount = 0;
    rag->resourceCount = 0;
    
    for (int i = 0; i < MAX_PROCESS; i++) {
        for (int j = 0; j < MAX_RESOURCE; j++) {
            rag->graph[i][j] = 0;
        }
    }
}

void addEdge(ResourceAllocationGraph* rag, int processIndex, int resourceIndex) {
    rag->graph[processIndex][resourceIndex] = 1;
}

bool isCyclePresentUtil(ResourceAllocationGraph* rag, bool visited[], bool inRecursionStack[], int process) {
    visited[process] = true;
    inRecursionStack[process] = true;
    
    for (int i = 0; i < rag->resourceCount; i++) {
        if (rag->graph[process][i] == 1) {
            if (!visited[i]) {
                if (isCyclePresentUtil(rag, visited, inRecursionStack, i)) {
                    return true;
                }
            }
            else if (inRecursionStack[i]) {
                return true;
            }
        }
    }
    
    inRecursionStack[process] = false;
    return false;
}

bool isCyclePresent(ResourceAllocationGraph* rag) {
    bool visited[MAX_PROCESS] = {false};
    bool inRecursionStack[MAX_PROCESS] = {false};
    
    for (int i = 0; i < rag->processCount; i++) {
        if (!visited[i]) {
            if (isCyclePresentUtil(rag, visited, inRecursionStack, i)) {
                return true;
            }
        }
    }
    
    return false;
}

int main() {
    ResourceAllocationGraph rag;
    initRAG(&rag);
    
    // User input for processes and resources
    printf("Enter the number of processes: ");
    scanf("%d", &rag.processCount);
    
    printf("Enter the number of resources: ");
    scanf("%d", &rag.resourceCount);
    
    printf("Enter the names of processes:\n");
    for (int i = 0; i < rag.processCount; i++) {
        printf("Process %d: ", i + 1);
        scanf("%s", rag.processes[i].name);
    }
    
    printf("Enter the names of resources:\n");
    for (int i = 0; i < rag.resourceCount; i++) {
        printf("Resource %d: ", i + 1);
        scanf("%s", rag.resources[i].name);
    }
    
    // User input for edges
    int processIndex, resourceIndex;
    char choice;
    
    do {
        printf("Enter the process index (0-%d): ", rag.processCount - 1);
        scanf("%d", &processIndex);
        
        printf("Enter the resource index (0-%d): ", rag.resourceCount - 1);
        scanf("%d", &resourceIndex);
        
        addEdge(&rag, processIndex, resourceIndex);
        
        printf("Add another edge? (y/n): ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');
    
    // Check for deadlock
    if (isCyclePresent(&rag)) {
        printf("Deadlock detected!\n");
    }
    else {
        printf("No deadlock detected.\n");
    }
    
    return 0;
}
