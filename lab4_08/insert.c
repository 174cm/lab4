#include <stdio.h> 
#include <stdlib.h> 
#include <mysql.h> 
int main() 
{ 
	MYSQL mysql; 
	MYSQL_RES* res; 
	MYSQL_ROW row; 
	char query[80], name[20], birth[12], gender; 
	int id, i, fields; 
	mysql_init(&mysql); 
	if (!mysql_real_connect(&mysql, "localhost","root", "1234", "testdb", 0, (char *)NULL, 0)) { 
		printf("MySQL Error %d: %s\n", 	mysql_errno(&mysql), mysql_error(&mysql));
		exit(1);
	}
	printf("Input student's id, name, birth(YYMMDD), gender(M/F).\n"); 
	scanf("%d %s %s %c", &id, name, birth, &gender); 
	sprintf(query, "insert into student values(%d,	\'%s\', \'%s\', \'%c\')", id, name, birth, gender); 
		if (mysql_query(&mysql, query)) { 
			printf("MySQL Error %d: %s\n", 	mysql_errno(&mysql), mysql_error(&mysql)); 
			exit(1); 
		} 
	if (mysql_query(&mysql, "select * from student")) { 
		printf("MySQL Error %d: %s\n", 	mysql_errno(&mysql), mysql_error(&mysql));
		exit(1); 
	} 
	res = mysql_use_result(&mysql); 
	fields = mysql_num_fields(res); 
	printf("Total student is as follows.\n"); 
	while ((row = mysql_fetch_row(res))) { 
		for (i = 0; i < fields; ++i) { 
			printf("%12s ", row[i]); 
		}
		printf("\n"); 
	} 
	mysql_free_result(res); 
	mysql_close(&mysql); 
	return 0; 
}
