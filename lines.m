
dataset = [1 0 0 0 0 0 0 0 0 1;
           0 1 0 0 0 0 0 0 1 0;
           1 1 2 1 1 1 1 2 1 1;
           0 0 0 1 0 0 1 0 0 0;
           0 0 0 0 1 1 0 0 0 0;
           0 0 0 0 1 1 0 0 0 0;
           0 0 0 1 0 0 1 0 0 0;
           0 0 1 0 0 0 0 1 0 0;
           0 1 0 0 0 0 0 0 1 0;
           1 0 0 0 0 0 0 0 0 1;]
       
%save dataset1.txt dataset -ascii

dlmwrite('dataset1.txt',dataset, 'delimiter', ' ')
%type('dataset1.txt')
