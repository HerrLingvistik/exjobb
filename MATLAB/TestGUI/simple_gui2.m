function simple_gui2
% SIMPLE_GUI2 Select a data set from the pop-up menu, then
% click one of the plot-type push buttons. Clicking the button
% plots the selected data in the axes.

%  Create and then hide the UI as it is being constructed.
f = figure('Visible','off','Position',[360,500,450,285]);

% Construct the components.
hplay1    = uicontrol('Style','pushbutton',...
             'String','Play sound1','Position',[315,220,70,25],...
             'Callback',@playsound1_Callback);
hplay2    = uicontrol('Style','pushbutton',...
             'String','Play sound2','Position',[315,180,70,25],...
             'Callback',@playsound2_Callback);
hplay3 = uicontrol('Style','pushbutton',...
             'String','Play sound3','Position',[315,135,70,25],...
             'Callback',@playsound3_Callback);

% Initialize the UI.
% Change units to normalized so components resize automatically.
f.Units = 'normalized';
ha.Units = 'normalized';
hplay1.Units = 'normalized';
hplay2.Units = 'normalized';
hplay3.Units = 'normalized';
htext.Units = 'normalized';
hpopup.Units = 'normalized';

% Assign the a name to appear in the window title.
f.Name = 'Simple GUI';

% Move the window to the center of the screen.
movegui(f,'center')

% Make the window visible.
f.Visible = 'on';

% Push button callbacks. Each callback plots current_data in the
% specified plot type.
function playsound1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[y,Fs] = audioread('sound1.wav');
sound(y,Fs);
end

function playsound2_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[y,Fs] = audioread('sound2.wav');
sound(y,Fs);
end

function playsound3_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[y,Fs] = audioread('sound3.wav');
sound(y,Fs);
end

end