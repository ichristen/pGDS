function compile(fname)
    gather(fname)
    
    global gatheredFiles;
    global gatheredMethods;
    global gatheredClasses;
    global gatheredEnums;
    global numWritten;
    
    numWritten = 0;
    
    global mexFolder;
    mexFolder = 'mex';
    mexFolder2 = 'build';
    
    if ~exist(mexFolder, 'dir')
        mkdir(mexFolder);
    end
    
    global mexFile;
    mexFile = 'mex_bridge.cpp';
    
    mexFileWithDir = [mexFolder filesep mexFolder2 filesep mexFile];
    
    fmex = fopen(mexFile, 'w');
    
    
    fprintf(fmex, [ '#ifdef MATLAB_MEX_FILE' newline ]);
    fprintf(fmex, [ '    #include "mex.h"' newline ]);
    fprintf(fmex, [ '#endif' newline newline ]);
    
    fprintf(fmex, [ '#include "class_handle.hpp"' newline newline ]);
    
    for file = gatheredFiles
        fprintf(fmex, [ '#include "' file{1} '"' newline ]);
    end
    
    fprintf(fmex, newline);
    
    fprintf(fmex, [ 'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {' newline ...
                    '    if (nlhs < 0 || nrhs < 0) {  mexErrMsgTxt("' mexFile ': Something has gone horribly wrong; expected natural numbers to be non-negative."); }' newline newline ...
                    '    if (nlhs > 1) {  mexErrMsgTxt("' mexFile ': C functions cannot return more than one value."); }' newline ...
                    '    if (nrhs < 1) {  mexErrMsgTxt("' mexFile ': Expected at least one argument."); }' newline newline ...
                    '    int cmd = (int)*mxGetPr(prhs[0]);' newline newline ...
                    ]);
%                     '    printf("HERE!");' newline newline ...
%                     '    printf("cmd = %%i", cmd);' newline newline ...
    
                
%     writeMethods(fmex, onlyMethods);
        
    for ii = 1:length(gatheredClasses)
        inheritMethods(ii);
    end
    
    deleteMethod.name = 'delete';
    deleteMethod.type = 'delete';
    deleteMethod.typespecial = '';
    deleteMethod.arguments = [];
    deleteMethod.description = 'MATLAB-specific... Deletes MEX pointer reference.';
    deleteMethod.str = 'delete()';
    
    

    argument.type = 'new';
    argument.typespecial = '';
    argument.typedereference = '';
    argument.var = 'new';
    argument.default = '';
    
    newMethod.name = 'new';
    newMethod.type = 'new';
    newMethod.typespecial = '';
    newMethod.arguments = argument;
    newMethod.description = 'MATLAB-specific... Takes a pointer returned from MEX and turns it into an object.';
    newMethod.str = 'new(mexptr)';
    
    for ii = 1:length(gatheredClasses)
%         newMethod.type = gatheredClasses(ii).name;
        gatheredClasses(ii).methods = [deleteMethod gatheredClasses(ii).methods];
        
        for jj = 1:length(gatheredClasses(ii).methods)
            gatheredClasses(ii).methods(jj).namematlab = convertFunc(gatheredClasses(ii).methods(jj).name, length(gatheredClasses(ii).methods(jj).arguments));
        end
        
        newMethod.namematlab = gatheredClasses(ii).name;

        gatheredClasses(ii).methods = [newMethod gatheredClasses(ii).methods];
    end
    
    for jj = 1:length(gatheredMethods)
        gatheredMethods(jj).namematlab = convertFunc(gatheredMethods(jj).name, length(gatheredMethods(jj).arguments));
    end
    
    onlyMethods.methods = gatheredMethods;
    onlyMethods.name = 'pGDS';
    onlyMethods.parent = '';
    onlyMethods.variables = [];
    
%     gatheredClasses
    
    gatheredClasses = [gatheredClasses onlyMethods];
    
    for ii = 1:length(gatheredClasses)
        jj = 1;
        
        while jj <= length(gatheredClasses(ii).methods)
%             gatheredClasses(ii).methods(jj).name
            
            if isempty(strfind(gatheredClasses(ii).methods(jj).name, ':'))
                if ~isempty(interpretType(gatheredClasses(ii).methods(jj).type)) && ~strcmp('replaceIllegal', gatheredClasses(ii).methods(jj).name)     % If we recognize the return type...
                    kk = 1;

                    while kk <= length(gatheredClasses(ii).methods(jj).arguments)
                        interpreted = interpretType(gatheredClasses(ii).methods(jj).arguments(kk).type);

                        if isempty(interpreted)                                                         % If the type is not recognized...
                            if isempty(gatheredClasses(ii).methods(jj).arguments(kk).default)           % And there is no default...
                                disp(['Argument type "' gatheredClasses(ii).methods(jj).arguments(kk).type '" not recognized...']);
                                gatheredClasses(ii).methods(jj) = [];                                   % Then, ignore this method.
                                jj = jj - 1;
                                break;
                            else                                                                        % But if there is a default...
                                gatheredClasses(ii).methods(jj).arguments(kk:end) = [];                 % Then simply disallow access to the unknown types (which have defaults).
                            end
                        end

                        if strcmp(interpreted, 'class') && isempty(gatheredClasses(ii).methods(jj).arguments(kk).typespecial)
                            gatheredClasses(ii).methods(jj).arguments(kk).typedereference = '*';
                            gatheredClasses(ii).methods(jj).arguments(kk).typespecial = '*';
                        end

                        kk = kk + 1;
                    end
                else
                    disp(['Return type "' gatheredClasses(ii).methods(jj).type '" not recognized...']);
                    gatheredClasses(ii).methods(jj) = [];
                    jj = jj - 1;
                end
            else
                disp(['Method name "' gatheredClasses(ii).methods(jj).name '" not valid...']);
                gatheredClasses(ii).methods(jj) = [];
                jj = jj - 1;
            end
            
            jj = jj + 1;
        end
    end
        
    for ii = 1:length(gatheredClasses)
        writeMethods(fmex, gatheredClasses(ii));
    end

    fprintf(fmex, '}');
    
    fclose(fmex);
    
    
    helperFile = 'class_handle.hpp';    % Move this helper file with the rest of the files such that it can be used when compiling...
    movefile([mexFolder filesep mexFolder2 filesep helperFile], helperFile);
    
    failed = false;
    
    try
        mex('-v', mexFile, 'vector.cpp', 'math.cpp', 'polyline.cpp', 'primitives.cpp',  'device.cpp', 'font.cpp', 'material.cpp', 'testdevices.cpp', 'boolean.cpp'); %, '-L/usr/local/opt/freetype/lib', '-I/usr/local/opt/freetype/include/freetype2/');
    catch err
        disp(err.message);
        failed = true;
    end
        
    %     mex('-v', mexFile, 'vector.cpp', 'math.cpp', 'polyline.cpp', 'primitives.cpp',  'device.cpp', 'font.cpp');

    movefile(mexFile, mexFileWithDir);
    movefile(helperFile, [mexFolder filesep mexFolder2 filesep helperFile]);
    
    if ~failed  % If we succeeded, move our compiled .mexmaci64 file into the mex folder, with the compiled .m files.
        movefile('mex_bridge.mexmaci64', [mexFolder filesep 'mex_bridge.mexmaci64']);
    end
end

function inheritMethods(classNum)
    global gatheredClasses
    if ~isempty(gatheredClasses(classNum).parent)
        for ii = 1:length(gatheredClasses)
            if ii ~= classNum && strcmp(gatheredClasses(classNum).parent, gatheredClasses(ii).name)
                inheritMethods(ii);

                gatheredClasses(classNum).methods = [gatheredClasses(classNum).methods gatheredClasses(ii).methods];
                
                gatheredClasses(classNum).parent = '';  % Mark the inheritance to be completed.
            end
        end
    end
end

function writeMethods(fmex, class)
    global mexFolder;
    
    matFile = [mexFolder filesep class.name '.m'];
    
    fmat = fopen(matFile, 'w');
    
%                   [ '%CLASS_INTERFACE Example MATLAB class wrapper to an underlying C++ class' newline ...
%                     'classdef class_interface < handle' newline ...
%                     '    properties (SetAccess = private, Hidden = true)' newline ...
%                     '        objectHandle; % Handle to the underlying C++ class instance' newline ...
%                     '    end' newline ...
%                     '    methods' newline]
    
%     fprintf(fmat, [ 'classdef ' class.name ' < handle' newline ...
%                     '    properties (SetAccess = public, Hidden = true)' newline ...
%                     '        objectHandle;' newline ...
%                     '    end' newline ...
%                     '    methods' newline]);
    
    fprintf(fmat, [ 'classdef ' class.name ' < handle' newline ]);
                
    if strcmp(class.name, 'pGDS')
        fprintf(fmat, [ '    methods (Static)' newline]);
    else
        fprintf(fmat, [ '    properties (SetAccess = public, Hidden = true)' newline ...
                        '        objectHandle;' newline ...
                        '    end' newline]);
        fprintf(fmat, [ '    methods' newline]);
    end
                
%                     '    properties (SetAccess = private, Hidden = true)' newline ...
    
    for ii = 1:length(class.methods)
        writeMethod(fmex, fmat, ii, class);
        
        if      strcmp(class.methods(ii).name, 'print')     % VECTOR-specific...
            class.methods(ii).namematlab = 'disp';
            writeMethod([], fmat, ii, class);
        elseif  strcmp(class.methods(ii).name, 'magn')      % VECTOR-specific...
            class.methods(ii).namematlab = 'length';
            writeMethod([], fmat, ii, class);
        end
    end
    
    fprintf(fmat, [ '    end' newline ...
                    'end']);
                
    fclose(fmat);
end

function writeMatlabCall(fmat, method, ii, tab, isClass)
%     fprintf(fmat, [ tab '            %% ' method.type newline ]);
% 
%     if strcmp(method.type, 'void') || strcmp(method.type, 'delete')
%         fprintf(fmat, [tab '            ' ]);
%     elseif strcmp(method.type, 'new')
%         fprintf(fmat, [tab '            this.objectHandle = ' ]);
%     elseif strcmp(method.type, 'list')
%         fprintf(fmat, [tab '            [toReturn{1:nargout}] = ' ]);
%     else
%         fprintf(fmat, [tab '            toReturn = ' ]);
%     end
% 
%     fprintf(fmat, [ 'mex_bridge(' num2str(ii) ]);
% 
%     if      strcmp(method.type, 'new')
%         fprintf(fmat, [ ', varargin{:});' newline ]);
%     elseif  strcmp(method.type, 'delete')
%         fprintf(fmat, [ ', this.objectHandle);' newline ]);
%     else
%         fprintf(fmat, [ ', this.objectHandle, varargin{:});' newline ]);
%     end
    
    fprintf(fmat, [ tab '            %% ' method.type newline ]);
    
    accountForThis = ~strcmp(method.type, 'new');
    
%     classPresent = false;
    
    for jj = 1:length(method.arguments)
        if strcmp(interpretType(method.arguments(jj).type), 'class')
%             classPresent = true;
            fprintf(fmat, [tab '            if nargin >= ' num2str(jj+accountForThis) ' && isa(varargin{' num2str(jj) '}, ''' method.arguments(jj).type ''')' newline ]);
            fprintf(fmat, [tab '                varargin{' num2str(jj) '} = varargin{' num2str(jj) '}.objectHandle;' newline ]);
            fprintf(fmat, [tab '            elseif nargin >= ' num2str(jj+accountForThis) newline ]);
            fprintf(fmat, [tab '                error(''Expected ' method.arguments(jj).type ''');' newline ]);
            fprintf(fmat, [tab '            end' newline ]);
        end
    end
    
%     if classPresent
%         fprintf(fmat, [tab '            for ii = 1:length(varargin{:}); varargin{ii} end' newline ]);
%     end
    
    type = interpretType(method.type);
    
    if isClass
        objHandle = ', this.objectHandle';
    else 
        objHandle = '';
    end

    if strcmp(type, 'void')
        if isempty(method.arguments)
            fprintf(fmat, [tab '            mex_bridge(' num2str(ii) ', this.objectHandle);' newline ]);
        else
            fprintf(fmat, [tab '            mex_bridge(' num2str(ii) ', this.objectHandle, varargin{:});' newline ]);
        end
    elseif strcmp(type, 'delete')
        fprintf(fmat, [tab '            mex_bridge(' num2str(ii) ', this.objectHandle);' newline ]);
    elseif strcmp(type, 'new')
%         fprintf(fmat, [tab '            varargin' newline ]);
%         fprintf(fmat, [tab '            ~isempty(varargin)' newline ]);
%         fprintf(fmat, [tab '            isstruct(varargin{1})' newline ]);
%         fprintf(fmat, [tab '            if ~isempty(varargin) && isstruct(varargin{1})' newline ]);
% %         fprintf(fmat, [tab '                varargin{1}.ptr;' newline ]);
%         fprintf(fmat, [tab '                this.objectHandle = varargin{1}.ptr;' newline ]);
%         fprintf(fmat, [tab '            if isempty(varargin)' newline ]); 

        if isempty(method.arguments)
            fprintf(fmat, [tab '            this.objectHandle = mex_bridge(' num2str(ii) ');' newline ]);
        elseif strcmpi(method.arguments(1).type, 'new')
            fprintf(fmat, [tab '            this.objectHandle = varargin{1}.ptr;' newline ]);
        else
%         fprintf(fmat, [tab '            else' newline ]);
            fprintf(fmat, [tab '            this.objectHandle = mex_bridge(' num2str(ii) ', varargin{:});' newline ]);
        end

%         if isempty(method.arguments)
%             fprintf(fmat, [tab '                mex_bridge(' num2str(ii) ', this.objectHandle);' newline ]);
%         else
%             fprintf(fmat, [tab '                mex_bridge(' num2str(ii) ', this.objectHandle, varargin{:});' newline ]);
%         end

%         fprintf(fmat, [tab '            end' newline ]);
    elseif strcmp(type, 'list')
        fprintf(fmat, [tab '            [toReturn{1:nargout}] = mex_bridge(' num2str(ii) objHandle ', varargin{:});' newline ]);
    elseif strcmp(type, 'class')
        if isempty(method.arguments)
            fprintf(fmat, [tab '            structure.ptr = mex_bridge(' num2str(ii) objHandle ');' newline ]);
        else
            fprintf(fmat, [tab '            structure.ptr = mex_bridge(' num2str(ii) objHandle ', varargin{:});' newline ]);
        end
        fprintf(fmat, [tab '            toReturn = ' method.type '(structure);' newline ]);
    else
%         fprintf(fmat, [tab '            toReturn = mex_bridge(' num2str(ii) ', this.objectHandle, varargin{:});' newline ]);
        
        if isempty(method.arguments)
            fprintf(fmat, [tab '            toReturn = mex_bridge(' num2str(ii) objHandle ');' newline ]);
        elseif ~isempty(method.arguments(1).default)
            fprintf(fmat, [tab '            if isempty(varargin)' newline ]);
            fprintf(fmat, [tab '                toReturn = mex_bridge(' num2str(ii) objHandle ');' newline ]);
            fprintf(fmat, [tab '            else' newline ]);
            fprintf(fmat, [tab '                toReturn = mex_bridge(' num2str(ii) objHandle ', varargin{:});' newline ]);
            fprintf(fmat, [tab '            end' newline ]);
        else
            fprintf(fmat, [tab '            toReturn = mex_bridge(' num2str(ii) objHandle ', varargin{:});' newline ]);
        end
    end
end

function writeMethod(fmex, fmat, ii, class)
    global numWritten;
    global mexFile;
    
    method = class.methods(ii);
    
    className = class.name;
    
    this = 'this, ';
    
    if strcmp(className, 'pGDS')
        className = '';
        this = '';
    end
    
    % MATLAB ======================================================================================
    
%     for method.namematlab

    samename = [];

    for jj = 1:length(class.methods)
%         class
%         class.methods(jj)
        if strcmp(class.methods(jj).namematlab, class.methods(ii).namematlab)
            samename = [samename jj];   %#ok
        end
    end
    
    accountForThis = ~strcmp(class.methods(ii).type, 'new');
    
    if length(samename) == 1 || ii == samename(1)
        if      strcmp(class.methods(ii).type, 'new')
%             class.methods(ii)
            fprintf(fmat, [ '        function this = ' method.namematlab '(varargin)' ]);
        elseif  strcmp(class.methods(ii).type, 'delete')
%             class.methods(ii)
            fprintf(fmat, [ '        function delete(this)' ]);
        elseif  strcmp(class.methods(ii).type, 'void')
            fprintf(fmat, [ '        function ' method.namematlab '(' this 'varargin)' ]);
        else
            fprintf(fmat, [ '        function toReturn = ' method.namematlab '(' this 'varargin)' ]);
        end
    end
    
    if length(samename) == 1    % If ii is the only one of this name...
        fprintf(fmat, [' %% ' class.methods(ii).str ' - ' class.methods(ii).description newline]);
        
        writeMatlabCall(fmat, class.methods(ii), numWritten+1-isempty(fmex), '', ~isempty(className))
            
        if strcmp(class.methods(ii).namematlab, 'disp')
            fprintf(fmat, [ '            disp(newline)' newline ]);
        end
%         fprintf(fmat, [ '            [toReturn{1:nargout}] = mex_bridge(' num2str(numWritten+1) ', this.objectHandle, varargin{:});' newline ]);
    elseif ii == samename(1)    % If ii is the first of this name, with potentially-many following...
        fprintf(fmat, [' %% ' class.methods(ii).type ' ' class.methods(ii).name '(...), an overloaded method.' newline]);
        
        for jj = 1:length(samename)
            numWithoutDefault = 0;
            
            for kk = 1:length(class.methods(samename(jj)).arguments)
                if isempty(class.methods(samename(jj)).arguments(kk).default)
                    numWithoutDefault = numWithoutDefault + 1;
                end
            end
            
            if numWithoutDefault == length(class.methods(samename(jj)).arguments)
                logic = ['(nargin == ' num2str(length(class.methods(samename(jj)).arguments)+accountForThis) ') && '];
            else
                logic = ['(nargin >= ' num2str(numWithoutDefault+accountForThis) ' && nargin <= ' num2str(length(class.methods(jj).arguments)+accountForThis) ') && '];
            end
            
            for kk = 1:length(class.methods(samename(jj)).arguments)
                if isempty(class.methods(samename(jj)).arguments(kk).default)
                    logic = [logic '('];                                                %#ok
                else
                    logic = [logic '(nargin < ' num2str(kk+accountForThis) ' || '];                    %#ok
                end
                
                switch interpretType(class.methods(samename(jj)).arguments(kk).type)
                    case 'class'
                        logic = [logic 'isa(varargin{' num2str(kk) '}, ''' class.methods(samename(jj)).arguments(kk).type ''')) && '];       %#ok
                    case 'enum'
                        logic = [logic 'isa(varargin{' num2str(kk) '}, ''' class.methods(samename(jj)).arguments(kk).type ''')) && '];       %#ok
                    case 'string'
                        logic = [logic 'ischar(varargin{' num2str(kk) '})) && '];       %#ok
                    case 'new'
                        logic = [logic 'isstruct(varargin{' num2str(kk) '})) && '];       %#ok
                    case 'static'
                        logic = [logic 'isnumeric(varargin{' num2str(kk) '}) || islogical(varargin{' num2str(kk) '})) && '];    %#ok
                end
                
%                 if 
%                     logic = [logic '() && '];    %#ok
%                 end
            end
            
            if jj == 1
                fprintf(fmat, [ '            if ' logic(1:end-4) ' %% ' class.methods(samename(jj)).str ' - ' class.methods(samename(jj)).description newline]);
            else
                fprintf(fmat, [ '            elseif ' logic(1:end-4) ' %% ' class.methods(samename(jj)).str ' - ' class.methods(samename(jj)).description newline]);
            end
            
            writeMatlabCall(fmat, class.methods(samename(jj)), numWritten+1+samename(jj)-ii-isempty(fmex), '    ', ~isempty(className));
        
%             if strcmp(class.methods(jj).type, 'void') ||strcmp(class.methods(jj).type, 'delete')
%                 fprintf(fmat,   '                '  );
%             elseif strcmp(class.methods(jj).type, 'new')
%                 fprintf(fmat,   '                this.objectHandle = '  );
%             else
%                 fprintf(fmat,   '                [toReturn{1:nargout}] = '  );
%             end
%             
%             fprintf(fmat, [ 'mex_bridge(' num2str(numWritten+1+samename(jj)-ii) ]);
%                 
%             if      strcmp(class.methods(jj).type, 'new')
%                 fprintf(fmat, [ ', varargin{:});' newline ]);
%             elseif  strcmp(class.methods(jj).type, 'delete')
%                 fprintf(fmat, [ ', this.objectHandle);' newline ]);
%             else
%                 fprintf(fmat, [ ', this.objectHandle, varargin{:});' newline ]);
%             end
            
            if jj == length(samename)
                fprintf(fmat, [ '            end' newline ]);
            end
        end
    end
        
    if length(samename) == 1 || ii == samename(1)
        fprintf(fmat, [ '        end' newline ]);
    end
    
    % MEX ======================================================================================
    if ~isempty(fmex) && ~(length(method.arguments) == 1 && strcmp(method.arguments(1).type, 'new'))
        inClass = ~isempty(className) && ~strcmp(method.type, 'new');

        % Begin command definition
        fprintf(fmex, [ '    if (cmd == ' num2str(numWritten+1) ') { // ' method.str ' - ' method.description newline ]);

    %     fprintf(fmex, ['        printf("nlhs = %%i, nrhs = %%i", nlhs, nrhs);' newline newline]);

        % Return num check
        if (strcmp(method.type, 'void') && ~any(method.typespecial(1:end-1) == '*')) || strcmp(method.type, 'delete')
            fprintf(fmex, [ '        if (nlhs != 0) {  mexErrMsgTxt("' mexFile ': Expected no return values; void type."); }' newline ]);
        else
            fprintf(fmex, [ '        if (nlhs != 1) {  mexErrMsgTxt("' mexFile ': Expected 1 return value."); }' newline ]);
        end

        % Argument num check
        numArg = length(method.arguments);
        numDef = 0;

        for ii = 1:numArg
            numDef = numDef + ~isempty(method.arguments(ii).default);
        end

        if numArg == 0
            fprintf(fmex, [ '        if (nrhs != ' num2str(1+inClass) ') {  mexErrMsgTxt("' mexFile ': Expected exactly ' num2str(1+inClass) ' argument."); }' newline ]);
        else
            if numDef == 0
                fprintf(fmex, [ '        if (nrhs != ' num2str(numArg+1+inClass) ') {  mexErrMsgTxt("' mexFile ': Expected exactly ' num2str(numArg+1+inClass) ' arguments."); }' newline ]);
            else
                fprintf(fmex, [ '        if (nrhs < ' num2str(numArg-numDef+1+inClass) ' || nrhs > ' num2str(numArg+1+inClass)  ') {  mexErrMsgTxt("' mexFile ': Expected between ' num2str(numArg-numDef+1+inClass) ' and ' num2str(numArg+1+inClass) ' arguments."); }' newline ]);
            end
        end

        fprintf(fmex, newline);

        if inClass && ~strcmp(method.type, 'delete')
            fprintf(fmex, [ '        ' className '* ptr = convertMat2Ptr<' className '>(prhs[1]);' newline newline]);
        end

    %     if numArg ~= 0
            if numDef == 0
                % Print Arguments
                args = '';

                for ii = 1:numArg
                    argument = method.arguments(ii);

                    writeArgument(fmex, argument, ii+inClass, '');
    %                 fprintf(fmex, [ '        ' argument.type argument.typespecial ' ' argument.var ' = (' argument.type argument.typespecial ')*mxGetPr(prhs[' num2str(ii+inClass) ']);' newline ]);

                    args = [args argument.typedereference];

                    if ii == numArg
                        args = [args argument.var];         %#ok
                        fprintf(fmex, newline);
                    else
                        args = [args argument.var ', '];    %#ok
                    end
                end

                if strcmp(method.type, 'new')
                    fprintf(fmex, [ '        plhs[0] = convertPtr2Mat<' className '>( new ' className '(' args ') );' newline]);
                elseif strcmp(method.type, 'delete')
    %                 fprintf(fmex, [ '        delete ptr;' newline]);
                    fprintf(fmex, [ '        destroyObject<' className '>(prhs[1]);' newline ]);
                else
                    if inClass
                        if strcmp(method.type, 'void')
                            fprintf(fmex, [ '        ptr->' method.name '(' args ');' newline]);
                        else
                            fprintf(fmex, [ '        ' method.type method.typespecial ' toReturn = ptr->' method.name '(' args ');' newline]);
    %                         fprintf(fmex, [ '        static ' method.type method.typespecial ' toReturn = ptr->' method.name '(' args ');' newline]);
                            writeReturn(fmex, method.type, method.typespecial, '');
                        end
                    else
                        if strcmp(method.type, 'void')
                            fprintf(fmex, [ '        ' method.name '(' args ');' newline]);
                        else
                            fprintf(fmex, [ '        ' method.type method.typespecial ' toReturn = ' method.name '(' args ');' newline]);
    %                         fprintf(fmex, [ '        static ' method.type method.typespecial ' toReturn = ' method.name '(' args ');' newline]);
                            writeReturn(fmex, method.type, method.typespecial, '');
                        end
                    end
                end
            else
                for ii = numDef : -1 : 0
                    fprintf(fmex, [ '        if (nrhs == ' num2str(numArg-ii+1+inClass) ') {' newline ]);

                    args = '';

                    % Print Arguments
                    for jj = 1:numArg
                        argument = method.arguments(jj);

                        if jj <= numArg - ii
                            writeArgument(fmex, argument, jj+inClass, '    ');
    %                         fprintf(fmex, [ '            ' argument.type argument.typespecial ' ' argument.var ' = (' argument.type argument.typespecial ')*mxGetPr(prhs[' num2str(jj+inClass) ']);' newline ]);

    %                         if strcmp(argument.typespecial, '**')
    %                             args = [args '*'];
    %                         end


                            args = [args argument.typedereference];

                            if jj == numArg - ii
                                args = [args argument.var];         %#ok
    %                             fprintf(fmex, newline);
                            else
                                args = [args argument.var ', '];    %#ok
                            end
                        else
                            fprintf(fmex, [ '            // ' argument.type argument.typespecial ' ' argument.var ' = ' argument.default ';' newline ]);
                        end
                    end

                    fprintf(fmex, newline);

                    if strcmp(method.type, 'new')
                        fprintf(fmex, [ '            plhs[0] = convertPtr2Mat<' method.name '>( new ' method.name '(' args ') );' newline]);
                    elseif strcmp(method.type, 'delete')

                    else
                        if inClass
                            if strcmp(method.type, 'void')
                                fprintf(fmex, [ '            ptr->' method.name '(' args ');' newline]);
                            else
                                fprintf(fmex, [ '            ' method.type method.typespecial ' toReturn = ptr->' method.name '(' args ');' newline]);
    %                             fprintf(fmex, [ '            static ' method.type method.typespecial ' toReturn = ptr->' method.name '(' args ');' newline]);
    %                             fprintf(fmex, [ '            double toReturnDouble = (double)toReturn;' newline]);
    %                             fprintf(fmex, [ '            mxSetPr(plhs[0], &toReturnDouble)' newline]);
                                writeReturn(fmex, method.type, method.typespecial, '    ');
                            end
                        else
                            if strcmp(method.type, 'void')
                                fprintf(fmex, [ '            ' method.name '(' args ');' newline]);
                            else
                                fprintf(fmex, [ '            ' method.type method.typespecial ' toReturn = ' method.name '(' args ');' newline]);
    %                             fprintf(fmex, [ '            static ' method.type method.typespecial ' toReturn = ' method.name '(' args ');' newline]);
                                writeReturn(fmex, method.type, method.typespecial, '    ');
                            end
                        end
                    end

                    fprintf(fmex, [ '        }' newline ]);
                end
            end
    %     end

        fprintf(fmex, [ newline '        return;' newline ]);

        fprintf(fmex, [ '    }' newline ]);

        numWritten = numWritten+1;
    end
end

function writeArgument(fmex, argument, ii, tab)
    switch interpretType(argument.type)
        case 'class'
            fprintf(fmex, [ '        ' tab argument.type argument.typespecial ' ' argument.var ' = convertMat2Ptr<' argument.type '>(prhs[' num2str(ii) ']);' newline]);
        case 'enum'
            % will support...
        case 'string'
            fprintf(fmex, [ '        ' tab 'char str' num2str(ii) '[64];' newline ]);  % Make longer?
            fprintf(fmex, [ '        ' tab 'mxGetString(prhs[' num2str(ii) '], str' num2str(ii) ', 64);' newline ]);
            fprintf(fmex, [ '        ' tab 'std::string ' argument.var ' = std::string(str' num2str(ii) ');' newline ]);
%         case 'static'
        otherwise
            if argument.typespecial == '*'
                fprintf(fmex, [ '        ' tab argument.type argument.typespecial ' ' argument.var ' = (' argument.type argument.typespecial ')mxGetPr(prhs[' num2str(ii) ']);' newline ]);
            else
                fprintf(fmex, [ '        ' tab argument.type ' ' argument.var ' = (' argument.type ')*mxGetPr(prhs[' num2str(ii) ']);' newline ]);
            end
    end
end

function writeReturn(fmex, type, typespecial, tab)
    switch interpretType(type)
        case 'class'
            if typespecial == '*'
                fprintf(fmex, [ '        ' tab 'plhs[0] = convertPtr2Mat<' type '>(toReturn);' newline]);
            else
%                 fprintf(fmex, [ '        ' tab 'plhs[0] = convertPtr2Mat<' type '>(&toReturn);' newline]);
                fprintf(fmex, [ '        ' tab type '* toReturnPtr = new ' type '();' newline]);
                fprintf(fmex, [ '        memcpy(toReturnPtr, &toReturn, sizeof(' type '));' newline]);  % This is a bad way to do this...
                fprintf(fmex, [ '        ' tab 'plhs[0] = convertPtr2Mat<' type '>(toReturnPtr);' newline]);
            end
        case 'enum'
            % will support...
        case 'string'
            fprintf(fmex, [ '        ' tab 'plhs[0] = mxCreateString(toReturn.c_str());' newline ]);
%         case 'static'
        otherwise
%             fprintf(fmex, [ '        ' tab 'double toReturnDouble = (double)toReturn;' newline]);
%             fprintf(fmex, [ '        ' tab 'mxSetPr(plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL), &toReturnDouble);' newline]);
            fprintf(fmex, [ '        ' tab 'plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);' newline]);
            fprintf(fmex, [ '        ' tab 'double* toReturnDouble = mxGetPr(plhs[0]);' newline]);
            fprintf(fmex, [ '        ' tab '*toReturnDouble = (double)toReturn;' newline]);
    end
end

function str = convertFunc(func, narg)
    if length(func) > 8 && strcmp(func(1:8), 'operator')
        func(9:end)
        
        if narg == 0
            switch func(9:end)
                % matlab-supported
                case '+';	str = 'uplus';
                case '-';	str = 'uminus';

                case '!';	str = 'not';
                case '~';	str = 'not';

                % non-matlab-supported
                case '++';	str = 'preincriment';
                case '--';	str = 'predecriment';

                case '*';	str = 'dereference';
                case '&';	str = 'reference';
            end
        end

        if narg == 1
            switch func(9:end)
                % matlab-supported
                case '+';	str = 'plus';
                case '-';	str = 'minus';
                case '*';	str = 'mtimes';
                case '/';	str = 'mrdivide';

                case '==';	str = 'eq';
                case '!=';	str = 'ne';
                case '>';	str = 'gt';
                case '<';	str = 'lt';
                case '>=';	str = 'ge';
                case '<=';	str = 'le';

                case '&&';	str = 'and';
                case '||';	str = 'or';

                case '&';	str = 'and';
                case '|';	str = 'or';
                case '^';	str = 'mpower';
    %             case '^';	str = 'xor';
                case '[]';	str = 'subsindex';

                % Non-matlab-supported
                case '=';   str = 'set';
                case '%';	str = 'modulo';
                case '++';	str = 'incriment';
                case '--';	str = 'decriment';

                case '<<';	str = 'lshift';
                case '>>';	str = 'rshift';

                case '+=';	str = 'plusequals';
                case '-=';	str = 'minusequals';
                case '*=';	str = 'timesequals';
                case '/=';	str = 'divideequals';
                case '%=';	str = 'moduloequals';
                case '&=';	str = 'andequals';
                case '|=';	str = 'orequals';
                case '^=';	str = 'xorequals';
                case '<<=';	str = 'lshiftequals';
                case '>>=';	str = 'rshiftequals';

                case '->';	str = 'structdereference';
                case '->*';	str = 'dereferenceofstructdereference';
    %             case '()';	str = 'functioncall';
                case ',';	str = 'comma';
    %             case '""_b';	str = 'literal';
    %             case 'type';	str = 'statictypecast';
                case 'new';     str = 'allocate';
                case 'new[]';	str = 'allocatearray';
%                 case 'delete';	str = 'delete';
%                 case 'delete[]';str = 'deletearray';
                case 'delete';	str = 'deallocate';
                case 'delete[]';str = 'deallocatearray';

            end
        end
    else
        str = func;
    end
end

function str = interpretType(type)
    global gatheredClasses;
    global gatheredEnums;

    % Check if it is a class...
    for class = gatheredClasses
        if strcmp(type, class.name)
            str = 'class'; return;
        end
    end
    
    % Check if it is an enum...
    for enum = gatheredEnums
        if strcmp(type, enum{1})
            str = 'enum'; return;
        end
    end
    
    % Check to see if it is one of several std:: or static types
    switch type
        case 'std::string'
            str = 'string'; return;
        case {'new', 'delete', 'void'}
            str = type; return;
        case {'bool', 'char', 'int', 'long', 'float', 'double', 'GLdouble',...
              'int16_t', 'int32_t', 'uint16_t', 'uint32_t', 'uint64_t', 'size_t'}    % Don't deal with unsigned/etc at the moment...
            str = 'static'; return;
        otherwise
            str = ''; return;
    end
    
end

function gather(fname)
    C = strsplit(fname, '.');
    
    if length(C) == 2
        suffix = C{end};
    else
        suffix = '';
    end
    
    global gatheredFiles;
    global gatheredMethods;
    global gatheredClasses;
    global gatheredEnums;
    
    ii = 1;
%     done = false;
    % if fname
    
    if ~exist(fname)
        fname = ['devices' filesep fname];
    end
    
    if ~exist(fname)
        disp('Cannot interpret unknown filename...');
        return;
    end
    
    text = fileread(fname);
    l = length(text);
    
    currentClass = [];
    
    isHeader = strcmp(suffix, 'hpp') || strcmp(suffix, 'h');
    
    if (isHeader)
        gatheredFiles = [gatheredFiles {fname}];
    end
    

    while ii <= l   % Iterate through the characters in the file...
        
        if testText('#include')
            jj = 0;

            while getChar(jj) == ' '; jj = jj + 1; end              % While whitespace...

            if      getChar(jj) == '"'                              % If we find a begin-quote...
                kk = 1;
                while getChar(jj + kk) ~= '"'; kk = kk + 1; end     % Find the end-quote...

                fname2 = text(ii+jj+1 : ii+jj+kk-1);
                
                alreadyCompiled = false;
                
                for file = gatheredFiles
                    if strcmp(fname2, file{1})
                        alreadyCompiled = true;
                    end
                end
                
                if ~alreadyCompiled     % Or use includeguard?
                    gather(fname2);
                end

            elseif  getChar(jj) == '<'
                kk = 1;
                while getChar(jj + kk) ~= '>'; kk = kk + 1; end     % Find the end-bracket...
            end

        elseif testText('//') || getChar(0) == '#'
            jj = 0;
            while getChar(jj) ~= newline; jj = jj + 1; end
            ii = ii + jj;
        elseif isHeader
            
            if testText('enum')
                while getChar(jj) ~= ';' && getChar(jj) ~= '{'; jj = jj + 1; end
                
                name = text(ii : ii+jj-1);
                
                gatheredEnums = [gatheredEnums {name}];     %#ok
                
                if getChar(jj) ~= '{'
                    ii = ii+jj;
                    s = ii+1;
                    passThrough('{', '}');
                    e = ii-1;               % Eventually add support...
                end
            elseif testText('public:')
                public = true;
            elseif testText('private:')
                public = false;
            elseif getChar(0) == '{'
                passThrough('{', '}');
            elseif getChar(0) == '}'
                if ~isempty(currentClass)
                    gatheredClasses = [gatheredClasses currentClass];   %#ok
                    
                    currentClass = [];
                else
                    error([fname ': Unexpected end-curly bracket...'])
                end
            elseif getChar(0) == '('    % Found function definition...
                jj = 0;
                while getChar(jj) ~= ';' && ...
                     ~(getChar(jj) == ':' && ~(getChar(jj-1) == ':' || getChar(jj+1) == ':')) && ...
                      getChar(jj) ~= '#' && ...
                      getChar(jj) ~= '}' && ...
                     ~(getChar(jj-1) == '/' && getChar(jj) == '/') && ...
                     ~(getChar(jj-1) == '*' && getChar(jj) == '/')
                  jj = jj - 1; 
                end
                    
                kk = 0;
                while ~(getChar(jj+kk-1) == '/' && getChar(jj+kk) == '/') && getChar(jj+kk) ~= 10
                    kk = kk - 1;
                end
                
%                 'HERE'
                
                if (getChar(jj+kk-1) == '/' && getChar(jj+kk) == '/')  % If the character we stopped on was actually inside a comment...
%                     'THERE'
                    
                    jj = jj+kk;
                    while getChar(jj) ~= 10 && jj < 0
                        jj = jj + 1;
                    end
                    
                    if jj == 0
                        error('Newline was not found; discovered function was commented...');
                    end
                end
                
                if getChar(jj) == '#' || (getChar(jj-1) == '/' && getChar(jj) == '/')
                    while getChar(jj) ~= newline; jj = jj + 1; end
                end
                jj = jj + 1;
                
                s = ii+jj;

                passThrough('(', ')');
                
                fullFuncDef = strtrim(text(s : ii));
                
                jj = 0;
                
                while getChar(jj) ~= newline; jj = jj + 1; end
                
                e = ii + jj - 1;
                
                while ~(getChar(jj-1) == '/' && getChar(jj) == '/') && ~(getChar(jj-1) == '/' && getChar(jj) == '*') && jj > 0; jj = jj - 1; end
                
                description = '';
                
                if jj > 0
                    description = strtrim(text(ii+jj+1 : e));
                end
                
                lpara = strfind(fullFuncDef, '(');
                rpara = strfind(fullFuncDef, ')');
                
                argumentstr = fullFuncDef(lpara(1)+1 : rpara(end)-1);
                
                start = 1;
                
                numAngle = 0;
                numPara = 0;
                numCurly = 0;
                
                argumentstrs = {};
                
                for kk = 1:length(argumentstr)
                    if      argumentstr(kk) == '<'
                        numAngle = numAngle + 1;
                    elseif  argumentstr(kk) == '>'
                        numAngle = numAngle - 1;
                    elseif  argumentstr(kk) == '('
                        numPara = numPara + 1;
                    elseif  argumentstr(kk) == ')'
                        numPara = numPara - 1;
                    elseif  argumentstr(kk) == '{'
                        numCurly = numCurly + 1;
                    elseif  argumentstr(kk) == '}'
                        numCurly = numCurly - 1;
                    elseif numAngle == 0 && numPara == 0 && numCurly == 0 && argumentstr(kk) == ','
                        argumentstrs = [argumentstrs {argumentstr(start : kk-1)}];  %#ok
                        start = kk+1;
%                     elseif numAngle < 0
%                         error([fname ': Too many right angle brackets >.']);
                    end
                end
                
                argumentstrs = [argumentstrs {argumentstr(start : end)}];           %#ok
                
%                 arguments = strsplit(fullFuncDef(lpara(1)+1 : rpara(end)-1), {',', newline});

                func.arguments = [];
                
                for argument_ = argumentstrs
%                     argument = strtrim(argument);
                    
                    if ~isempty(argument_{1})
                        argumentstr = strtrim(strrep(argument_{1}, 'const', ''));      % Ignore const for now...

    %                     seplist = strfind(argument, {' ', '*', '&'});
                        seplist = regexp(argumentstr,' |*|&');

                        sep = seplist(end);

                        argument.type = strtrim(argumentstr(1 : sep));
%                         argument.type = strrep(argumentstr(1 : sep), ' ', '');
                        
                        argument.typespecial = '';
                        argument.typedereference = '';

                        for special = '*&'
                            if argument.type(end) == special
                                argument.type = argument.type(1:end-1);
                                argument.typespecial = special;
                            end
                        end
                        
                        C = strsplit(argumentstr(sep+1 : end), '=');
                        
                        argument.var = C{1};
                        
                        argument.default = '';
                        
                        if length(C) == 2
%                             try
%                                 argument.default = eval(C{2});
%                             catch
%                                 
%                             end
                            
                            argument.default = C{2};
                        end
                        
                        func.arguments = [func.arguments argument];
                    end
                end
                
%                 postfix = fullFuncDef(rpara(end)+1 : end);  % Ignore for now...
                
                prefix = strtrim(strrep(strrep(fullFuncDef(1 : lpara(1)-1), 'const', ''), 'inline', ''));   % ignore these for now...
                
                C = strsplit(prefix, {' ', newline});
                
%                 func.arguments
                
                if length(C) == 2
                    func.name = C{2};
                    func.type = strrep(C{1}, ' ', '');
                elseif  length(C) == 1
                    if      strcmp(C{1}, currentClass.name)
                        func.name = C{1};
                        func.type = 'new';
                    elseif  strcmp(C{1}, ['~' currentClass.name])
%                         func.name = C{1};
                        func.name = 'delete';
                        func.type = 'delete';
                    else
                        error([fname ': Every function must have at least a name and a return type. ' fullFuncDef ' does not have this.'])
                    end
                elseif length(C) == 3 && strcmp(C{2}, 'operator')
                    func.name = [C{2} C{3}];    % operator
                    func.type = C{1};
                else
                    func.name = C{end};
                    func.type = strjoin(C(1 : end-1));
%                     error([fname ': odd function prefix']);
                end
                
                func.typespecial = '';
                
                for special = '*&'
                    if func.type(end) == special
                        func.type = func.type(1:end-1);
                        func.typespecial = special;
                    end
                end
                
                func.description = description;
                func.str = fullFuncDef;
                
                if isempty(currentClass)
                    gatheredMethods = [gatheredMethods func];   %#ok
                elseif public
                    currentClass.methods = [currentClass.methods func];
                end
                
            elseif testText('class')
                if ~isempty(currentClass)
                    error([fname ': Cannot define a new class until the definition of ' currentClass.name ' has finished.']);
                end
                
                public = false;
                
                jj = 0;

                while getChar(jj) ~= '{' && getChar(jj) ~= ';'; jj = jj + 1; end    % Wait until the class is defined ({) or if it is empty (;)
            
                className = strtrim(text(ii : ii+jj-1));
                inheritsFrom = '';
                
                C = strsplit(className, ':');
                
                if length(C) > 1
                    className =     strtrim(C{1});
                    
                    C2 = strsplit(strtrim(className), ' ');
                    
                    inheritsFrom =  strtrim(C2{end});
                end
                
%                 className
                
                if getChar(jj) == '{'
                    currentClass.name =         className;
                    currentClass.parent =       inheritsFrom;
                    currentClass.methods =      [];
                    currentClass.variables =    [];
                end
                
                ii = ii + jj;
            end
            
        end
        
        ii = ii + 1;
    end

    
    
    
    
    function tf = testText(test)
        if length(text) >= ii + length(test) && ii > 0
            tf = strcmp(text( ii : ii+length(test)-1 ), test);

            if tf
                ii = ii + length(test);
            end
        else
            tf = false;
        end
    end

    function char = getChar(jj)
        if length(text) >= ii + jj && ii + jj > 0
            char = text(ii + jj);
        else
            char = '';
        end
    end

    function passThrough(b, e)
        if text(ii) == b
            bracketNum = 1;
            ii = ii + 1;

            while bracketNum > 0 && ii <= l
                if      text(ii) == b
                    bracketNum = bracketNum + 1;
                elseif  text(ii) == e
                    bracketNum = bracketNum - 1;
                end

                ii = ii + 1;
            end

            if bracketNum > 0
                error([fname ': Could not find ' e '...'])
            end
        end
    end
end




