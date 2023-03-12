% small matrix to test, matrix X matrix with Fox...

% row = 4;
% col = 6;
% col2 = 8;
% 
% test_a = ones(row, col);
% test_b = ones(col, col2);

test_a = [
    5 7 11.0;
    13 17 19;
    23 29 31
    ];

test_b = [
    1 3 5;
    1 3 5;
    1 3 5
    ];
C = test_a*test_b;

writematrix(C, "format_c.txt");
writematrix(test_a, "format_a.txt");
writematrix(test_b, "format_b.txt");

res = norm(C);

% format like the prof
fid =fopen('small_a.txt', 'w' );
fprintf(fid, '3\n3\n');
for i=1:3
    for j=1:3
        fprintf(fid, '%f\n', test_a(i, j));
    end
end
fclose(fid);

fid =fopen('small_b.txt', 'w' );
fprintf(fid, '3\n3\n');
for i=1:3
    for j=1:3
        fprintf(fid, '%f\n', test_b(i, j));
    end
end
fclose(fid);

% fid =fopen('test_c.txt', 'w' );
% fprintf(fid, '%g\n%g\n', row, col2);
% for i=1:row
%     for j=1:col2
%         fprintf(fid, '%f\n', C(i, j));
%     end
% end
% fclose(fid);

norm(C)


