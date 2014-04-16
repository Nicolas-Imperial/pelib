//  printf("[%s:%s:%i] Running test '%s'... ", __FILE__, __FUNCTION__, __LINE__, #test);
#define test(test) \
  printf("Running test '%s'... ", #test); \
  test_setup(); \
  if(test_record(test())) \
  { \
    printf("passed\n"); \
  } \
  else \
  { \
    printf("failed\n"); \
  } \
  test_teardown();

// Neutralize the definition of NDEBUG to allow the use of assert()
#ifdef NDEBUG
#undef NDEBUG
#endif

void test_init();
void test_setup();
void test_teardown();
void test_finalize();
void test_run();
int test_record(int);