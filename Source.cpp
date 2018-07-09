#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct User
{
	char name[20];
	int id;
	int post_id = 0;
	int posts[10];
}user;
typedef struct Post
{
	int id;
	char name[20];
	char post_name[20];
	char content[970];
	char comments[5][10];
	int likes;
	int ncomments;
	
}post;
user user_details;
post post_details;
int main()
{
	FILE *file_store;
	file_store = fopen("filestore.bin", "rb+");
	/*int num_users = 0,posts=0;
	fwrite(&num_users, sizeof(int), 1, file_store);
	for (int i = 0; i < 100; i++)
	{
	char ch = '0';
	fwrite(&ch, sizeof(char), 1, file_store);
	}
	fseek(file_store, 104+sizeof(user)*10, SEEK_SET);
	fwrite(&posts,sizeof(int),1,file_store);
	fclose(file_store);
	file_store = fopen("filestore.bin", "rb+");
	fread(&num_users, sizeof(int), 1, file_store);
	for (int i = 0; i < 100; i++)
	{
	char ch;
	fread(&ch, sizeof(char), 1, file_store);
	printf("%c ", ch);
	}
	fseek(file_store, 104+sizeof(user)*10, SEEK_SET);
	fread(&posts,sizeof(int),1,file_store);
	fclose(file_store);
	*/
	
	while (1)
	{
	rewind(file_store);
		char uname[20];
		int ch,num_users,id;
		printf("1.create user\n2.Login\n");
		scanf("%d\n", &ch);
		if (ch == 1)
		{
			rewind(file_store);
			printf("Enter user details\n");
			printf("Enter name\n");
			scanf("%s", uname);
			fread(&num_users, sizeof(int), 1, file_store);
			user_details.id = num_users+1;
			id = num_users + 1;
			printf("User Created Successfully....Your id is %d\n", user_details.id);
			rewind(file_store);
			fwrite(&user_details.id, sizeof(int),1, file_store);
			strcpy(user_details.name, uname);
			user_details.post_id = 0;
			fseek(file_store, 104 + (id - 1) * sizeof(user), SEEK_SET);
			fwrite(&user_details, sizeof(user), 1, file_store);
		}
		if (ch == 2)
		{
			printf("Enter your id:\n");
			scanf("%d", &id);
			fseek(file_store, 104+(id-1)*sizeof(user), SEEK_SET);
			fread(&user_details, sizeof(user), 1, file_store);
			printf("%d %s\n", user_details.id, user_details.name);
		}
		while (1)
		{
			printf("1.Read all posts\n2.Create post\n");
			rewind(file_store);
			fseek(file_store, 784, SEEK_SET);
			scanf("%d", &ch);
			int post_block_id;
			if (ch == 1)
			{
				post post_temp;
				int num_posts;
				fread(&num_posts, sizeof(int), 1, file_store);
				for (int i = 0; i < num_posts; i++)
				{
					ftell(file_store);
					fread(&post_temp, sizeof(post), 1, file_store);
					printf("%d %s:\n", post_temp.id, post_temp.name);
					printf("%s\n", post_temp.post_name);
					printf("Likes:%d\n", post_temp.likes);
					printf("Comments:\n");
					for (int j = 0; j < post_temp.ncomments; j++)
					{
					printf("%s\n", post_temp.comments[j]);
					}
					printf("\n");
					printf("1.Like 2.Comment 3.Download\n4.Next\n");
					scanf("%d", &ch);
					int size=sizeof(post);
					if (ch == 1)
					{
						post_temp.likes++;
						fseek(file_store,-size, SEEK_CUR);
						fwrite(&post_temp, (sizeof(post)), 1, file_store);
					}
					else if (ch == 2)
					{
						char com[10];
						scanf("%s", com);
						strcpy(post_temp.comments[post_temp.ncomments], com);
						post_temp.ncomments++;
						fseek(file_store, -size, SEEK_CUR);
						fwrite(&post_temp, sizeof(post), 1, file_store);
					}
					else if (ch == 3)
					{
						printf("%s\n", post_temp.post_name);
						FILE *fp = fopen(post_temp.post_name, "w+");
						char c;
						int k = 0;
						c = post_temp.content[k];
						while (c != '\0')
						{
							fputc(c, fp);
							k++;
							c = post_temp.content[k];
						}
						fclose(fp);
					}
				}
			}
			else
			{
				char post_message[970];
				char postname[20];
				int num_posts;
				fread(&num_posts, sizeof(int), 1, file_store);
				fseek(file_store, 784, SEEK_SET);
				num_posts++;
				fwrite(&num_posts, sizeof(int), 1, file_store);
				//fseek(file_store, 0, SEEK_END);
				post_block_id = (num_posts-1)*sizeof(post) + 788;
				fseek(file_store, post_block_id, SEEK_SET);
				user_details.posts[user_details.post_id] = post_block_id;
				user_details.post_id++;
				printf("Enter the file name\n");
				scanf("%s", postname);
				FILE *fp=fopen(postname, "r+");
				char c;
				int k = 0;
				while ((c=fgetc(fp))!=EOF)
				{
					post_details.content[k++] = c;
				}
				fclose(fp);
				post_details.content[k] = '\0';
				post_details.id = id;
				post_details.likes = 0;
				post_details.ncomments = 0;
				strcpy(post_details.name, user_details.name);
				strcpy(post_details.post_name, postname);
				//strcpy(post_details.content, post_message);
				fseek(file_store, post_block_id, SEEK_SET);
				fwrite(&post_details, sizeof(post), 1, file_store);
				fflush(file_store);
			}
		}
	}
	
	getchar();
	return 0;
}