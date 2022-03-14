// countingSemUsingBinarySem.pdf 

// Semaphor is an integer variable;
// Semaphor is a synchronization tool that doesn't require busy waiting
// two atomic cooperation: wait(s) and signal(s)


// Implementation:
/*
wait(s) enter into thw while loop:
while (s <= 0 ) {
    do no operation;
    s --;
}

signal(s) indicates s ++

Semaphors as general synchronization tool
1. Countign semaphors: the int value can range over an
unresticted domain

2. Binary semaphors: the int value can range over b/w 0 and 1
which is simplier to do

do {
    // s should be initialized as 1
    wialt(s);
    // critical section --> execute the program (process P1)

    signal(s);
    // reminder section 
} whiel (condition);

wait(s){
    while (s <= 0){
        s --;
    }
}

signal(s){
    s ++;
}

CSem(K) cs { // counting sem, init K
int val ← K; // value of csem
BSem wait(0); // to block on csem
BSem mutex(1); // protects val
Pc(cs) {
P(mutex);
val ← val − 1;
if val < 0 {
V(mutex);
1: P(wait);
} else
V(mutex);
}ß
Vc(cs) {
P(mutex);
val ← val + 1;
if val ≤ 0
V(wait);
V(mutex);
}
}

*/

int CSem(K){ // counting sem, init K
    int val = K; // value of csem
    BSem wait(0); // to block on csem
    BSem mutex(1); // protects val

    Pc(cs) {
        P(mutex);
        val = val − 1;
        if (val < 0) {
            V(mutex);
            P(wait);
        } else{
            V(mutex);
        }
    }

    Vc(cs) {
        P(mutex);
        val = val + 1;
        if (val ≤ 0){
            V(wait);
        }
        V(mutex);
    }
}