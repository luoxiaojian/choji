mkdir('tasksets');
homedir='tasksets/';

for i=1:8
	dirname=strcat('p_', int2str(i*2));
	dirname=strcat(dirname, '/');
	dirname=strcat(homedir, dirname);
	mkdir(dirname);
	generation(i*4, i*2, 100, 1000, dirname);
end

disp('generation finished');