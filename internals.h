// INTERNALS ======================================================================================

// FUNCTIONS --------------------------------------------------------------------------------------
int doreq(  // Send a request
	char *var,  // Var to write to
	char *token,  // OAUTH token
	char *method,  // Request method (GET, POST, etc.)
	char *url,  // URL to send request to
	char *content,  // POST content
	int verbose  // 0: none; 1: errors only; 2: all
	);