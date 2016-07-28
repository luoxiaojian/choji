function choji(n, m, s, fname)
state=0;
while state==0
    state=1;
    [te, tp, r, hp]=generateTaskset(n, s, 5, 5, 30, 1);
    while r==0
        [te, tp, r, hp]=generateTaskset(n, s, 5, 5, 30, 1);
    end
    [ce, cp, r, hp]=generateTaskset(m, s, 5, 5, 20, 1);
    while r==0
        [ce, cp, r, hp]=generateTaskset(m, s, 5, 5, 20, 1);
    end
    at=0; ac=0;
    ft=zeros(1, m); fc=zeros(1, m);
    for i=1:n
        ft(i)=te(i)/tp(i);
    end
    for i=1:m
        fc(i)=ce(i)/cp(i);
    end
    ft=sort(ft, 'descend');
    fc=sort(fc, 'descend');
    for i=1:m
        at=at+ft(i);
        ac=ac+fc(i);
        if at>ac
            state=0;
            break
        end
    end
    for i=1:n
        ft(i)=te(i)/tp(i);
    end
    for i=1:m
        fc(i)=ce(i)/cp(i);
    end
    for i=1:n-1
        for j=(i+1):n
            if ft(i)<ft(j)
                tmp=te(i);
                te(i)=te(j);
                te(j)=tmp;
                tmp=tp(i);
                tp(i)=tp(j);
                tp(j)=tmp;
            end
        end
    end
    for i=1:m-1
        for j=(i+1):m
            if fc(i)<fc(j)
                tmp=ce(i);
                ce(i)=ce(j);
                ce(j)=tmp;
                tmp=cp(i);
                cp(i)=cp(j);
                cp(j)=tmp;
            end
        end
    end
    for i=1:n
        if tp(i)>30
            state=0;
            break
        end
    end
    for i=1:m
        if cp(i)>20
            state=0;
            break
        end
    end
end
fout=fopen(fname, 'w');
fprintf(fout, '%s %s\n', int2str(n), int2str(m));
for i=1:n
    fprintf(fout, '%s %s\n', int2str(te(i)), int2str(tp(i)));
end
for i=1:m
    fprintf(fout, '%s %s\n', int2str(ce(i)), int2str(cp(i)));
end
fclose(fout);
end