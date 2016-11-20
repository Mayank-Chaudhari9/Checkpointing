                    Extending a Checkpointing System
                ========================================
1 > File structure
    DMTCP code/
              |--insert.c
              |--newdbplugin.c      // plugin source code
              |--newplugin.so		// plugin executable
              |--print_table.c
              |--simpledb.c
              |--simpledb
              |--printtable
              |--con_struct.c
              |--connection
			  |--p2.c			//close plugin source 
			  |--close.so		// close plugin executable

2> Compiling the code
    i) install mysql.h headerfile
    ii) compile the mysql files with following command
            gcc -o output $(mysql_config --cflags) c_file $(mysql_config --libs)

    iii) compile the DMTCP plugin with following command
            gcc -shared -o close.so p2.c -fPIC
			gcc -shared -o newplugin.so newdbplugin.c -fPIC

3> Running the files
  i)start mysql server
  ii)configure the username and password
        username - root
        password - toor
  iii) run the dmtcp_coordinator
  iii)in new terminal run the database file with dmtcp plugin with following command

      dmtcp_launch --interval 5 --with-plugin ./newplugin.so:./close.so ./db_file_executable

	  
 


