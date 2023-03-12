% small matrix to test ...

row = 32;
col = 2;
col2 = 1;

test_a = ones(row, col);
test_b = ones(col, col2);

z = 1.1;
for i=1:row
    for j=1:col
        test_a(i, j) = z; 
        z = z + .05;
    end
end

z = 1.3;
for i=1:col
    for j=1:col2
        test_b(i, j) = z; 
        z = z + .1;
    end
end

C = test_a*test_b;

% writematrix(C, "format_c.txt");
% writematrix(test_a, "format_a.txt");
% writematrix(test_b, "format_b.txt");

% format like the prof
fid =fopen('test_a.txt', 'w' );
% fprintf(fid, '%g\n%g\n', row, col);
for i=1:row
    for j=1:col
        fprintf(fid, '%f\n', test_a(i, j));
    end
end
fclose(fid);

fid =fopen('test_b.txt', 'w' );
% fprintf(fid, '%g\n%g\n', col, col2);
for i=1:col
    for j=1:col2
        fprintf(fid, '%f\n', test_b(i, j));
    end
end
fclose(fid);

fid =fopen('test_c.txt', 'w' );
fprintf(fid, '%g\n%g\n', row, col2);
for i=1:row
    for j=1:col2
        fprintf(fid, '%f\n', C(i, j));
    end
end
fclose(fid);

res = norm(C);
fid =fopen('test_norm_c.txt', 'w' );
fprintf(fid, '%g\n', res);
fclose(fid);

% special for this example
fid =fopen('size.txt', 'w' );
fprintf(fid, '%g %g\n', row, col);
fclose(fid);

display(norm(C))


