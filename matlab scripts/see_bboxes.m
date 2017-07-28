%Authors:
%Ankit Dhall and Yash Chandak

%load digitStruct.mat
fid = fopen('ground_full.txt','w');
for i = 1:5000%length(digitStruct)
    im = imread([digitStruct(i).name]);
    fprintf(fid, '%d ',i);
    fprintf(fid, '%d ', length(digitStruct(i).bbox));
    for j = 1:length(digitStruct(i).bbox)
        
        [height, width] = size(im);
        aa = max(digitStruct(i).bbox(j).top+1,1);
        bb = min(digitStruct(i).bbox(j).top+digitStruct(i).bbox(j).height, height);
        cc = max(digitStruct(i).bbox(j).left+1,1);
        dd = min(digitStruct(i).bbox(j).left+digitStruct(i).bbox(j).width, width);
        
        fprintf(fid, '%d ',cc);
        fprintf(fid, '%d ',aa);
        
        fprintf(fid, '%d ',(dd-cc));
        fprintf(fid, '%d ',(bb-aa));
        
        %imshow(im(aa:bb, cc:dd, :));
        fprintf('%d\n',digitStruct(i).bbox(j).label );
        %pause;
    end
    fprintf(fid, '\r\n');
end
fclose(fid);
