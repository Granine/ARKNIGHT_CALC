//sanity Calc functions
int stageSanityCalc(int mode);
void inputChecker(char itemReport[20]);
int fullNamePrinter(FILE* SanityCalcHistory, char itemReport[20]);
double averageSanityFinder(char itemReport[20]);
double netSanity(double sanityReturn[20], int itemCount);

//shop functions
int shopCalculator(void);

void greenStore(char input[20]);
void yellowStore(char input[20]);
void redStore(char input[20]);
void orangeStore(char input[20]);
void fStore(char input[20]);
void holder1Store(char input[20]);
void holder2Store(char input[20]);

int storePromptReader(char input[20]);
void efficiencyOrderer(int size, char strItm[][12], double strCost[], int efficiencyOrder[]);
