#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

displayresult(MYSQL *mysql, char *condition)
{
char query[80];
MYSQL_RES* res;
MYSQL_ROW row;
int i, fields;
sprintf(query, "select * from lab4");

if (mysql_query(mysql, query)) {
	printf("MySQL Error %d: %s\n",
	mysql_errno(mysql), mysql_error(mysql));
	exit(1);
}
res = mysql_use_result(mysql);
fields = mysql_num_fields(res);
printf("\nID, TELL and ADDRESS\n");

while ((row = mysql_fetch_row(res))) {
	for (i = 0; i < fields; ++i) {
			printf("|     %s     |", row[i]);
		}
		printf("\n");
	}
	mysql_free_result(res);
}

int main()
{
MYSQL mysql;
MYSQL_RES* res;
MYSQL_ROW row;
char query[80], tell[20], address[50];
int id, i, fields;
mysql_init(&mysql);

if (!mysql_real_connect(&mysql, "localhost", "root", "1234", "lab4", 0, (char *)NULL, 0)) {
printf("MySQL Error %d: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
exit(1);
}
while(1){
	int choice;
	printf("\n1. search to id, tell and address\n");
	printf("2. insert to id, tell and address\n");
	printf("3. update to tell and address\n");
	printf("0. exit\n");
	printf("\nselect number : ");
	scanf("%d", &choice);
	switch(choice){
		case 1:
			displayresult(&mysql, ("%s", tell));
			break;
		case 2:
			printf("\nInput id, TELL, ADDRESS. \n");
			printf("ID : ");
			scanf("%d", &id);
			printf("\nTELL : ");
			scanf("%s", tell);
			printf("\nADDRESS : ");
			scanf("%s", address);
			sprintf(query, "insert into lab4 values(%d, \'%s\', \'%s\')", id, tell, address);
			if (mysql_query(&mysql, query)) {
				printf("MySQL Error %d: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
				exit(1);
			}
			else{
				printf("insert complete\n");
			}
			break;
		case 3:
			if (mysql_query(&mysql, "use test")){
				printf("Error %d: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
				exit(1);
			}
			printf("\nselect id and you want to TELL, ADDRESS.\n");
			printf("ID : ");
			scanf("%d", &id);
			printf("\nTELL : ");
			scanf("%s", tell);
			printf("\nADDRESS : ");
			scanf("%s", address);
			sprintf(query, "update lab4 set tell='%s', address='%s' where id='%d'", tell, address, id);
			if (mysql_query(&mysql, query)) {
				printf("MySQL Error %d: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
				exit(1);
			}
			else{
				printf("update complete\n");
			}
			break;
		case 0:
			mysql_close(&mysql);
			return 0;
		}
	}
}
