// PUBLICS ========================================================================================

// FUNCTIONS --------------------------------------------------------------------------------------
int init();

int clean();


// STRUCTS ----------------------------------------------------------------------------------------
typedef struct AccountCalendar {
	char *id;  // The ID of the account associated with this calendar
	char *name;  // The name of the account associated with this calendar
	char *parent_account_id;  // The account's parent ID, or null if this is the root account
	char *root_account_id;  // The ID of the root account, or null if this is the root account
	int visible;  // Whether this calendar is visible to users
	int auto_subscribe;  // Whether users see this calendar's events without needing to manually add it
	int sub_account_count;  // Number of this account's direct sub-accounts
	char *asset_string;  // Asset string of the account
	char *type;  // Object type
	char *calendar_event_url;  // URL to get full detailed events
	int can_create_calendar_events;  // Whether the user can create calendar events
	char *create_calendar_event_url;  // API path to create events for the account
	char *new_calendar_event_url;  // URL to open the more options event editor

} AccountCalendar;
AccountCalendar *malloc_AccountCalendar(  // Malloc AccountCalendar struct
	char *id,  // The ID of the account associated with this calendar
	char *name,  // The name of the account associated with this calendar
	char *parent_account_id,  // The account's parent ID, or null if this is the root account
	char *root_account_id,  // The ID of the root account, or null if this is the root account
	int visible,  // Whether this calendar is visible to users
	int auto_subscribe,  // Whether users see this calendar's events without needing to manually add it
	int sub_account_count,  // Number of this account's direct sub-accounts
	char *asset_string,  // Asset string of the account
	char *type,  // Object type
	char *calendar_event_url,  // URL to get full detailed events
	int can_create_calendar_events,  // Whether the user can create calendar events
	char *create_calendar_event_url,  // API path to create events for the account
	char *new_calendar_event_url  // URL to open the more options event editor
	);
int free_AccountCalendar(  // Free AccountCalendar struct
	AccountCalendar* obj  // AccountCalendar struct to free
	);