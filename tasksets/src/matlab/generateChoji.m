function generateChoji()
prefix='./chojies/';
postfix='.dat';
x='_';
for tn=6:2:20
    for k=1:1000
        pn=tn/2;
        s=round(pn/2);
        fname=int2str(tn);
        fname=strcat(fname, x);
        fname=strcat(fname, int2str(k));
        fname=strcat(fname, postfix);
        fname=strcat(prefix, fname);
        choji(tn, pn, s, fname);
    end   
end
end