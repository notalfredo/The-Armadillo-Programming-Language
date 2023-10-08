// Dalio, Brian A.
// dalioba
// 2023-04-10
//----------------------------------------------------------------
#if !defined( __EXIT_STATES_H__ )
#define __EXIT_STATES_H__

//----------------------------------------------------------------
enum {
  ES_OK = 0,          // No error occurred.

  ES_WRONGNUMARGS,    // Wrong number of command line arguments.
  ES_NOSRCFILE,       // Source file couldn't be accessed.
  ES_NOIRFILE,        // IR output file couldn't be created.
  ES_NOSCANNER,       // Scanner unable to be initialized.
  ES_PARSEFAILED,     // Syntactic error during parse.
  ES_SEMANTICFAILED,  // Semantic analysis error.
};

//----------------------------------------------------------------
#endif
