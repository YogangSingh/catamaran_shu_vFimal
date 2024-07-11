%% code to plot data from USV 2 Tests.
clear all
%% load data
%% CHANGE THE NAME OF THE FILE
%%load 'circular path.mat'

%%  
%%
% Assuming latitude and longitude are stored in arrays 'lat' and 'lon'
Xx = rdTrajectory.Xzero;
Yy = rdTrajectory.Yzero;
figure
%set(gcf,'Position',[100 100 500 500])
plot(Xx,Yy, "LineWidth",4)
title('$2nd Trajectory$','Interpreter','latex')
grid on
%set(gca, 'FontSize', 18, 'FontName', 'Times')
xlabel("$x(m)$", 'Interpreter','latex')
ylabel("$y(m)$",'Interpreter','latex')

%set(gca,'XMinorTick','on','YMinorTick','on')
axis equal
%%print("trajectory_fig.svg",'-dsvg')
%% get quarternions
 w =  (rdTrajectory.imuorientationw);
x =  (rdTrajectory.imuorientationx);
y =  (rdTrajectory.imuorientationy);
z =  (rdTrajectory.imuorientationz);
t = (rdTrajectory.elapsedTime);

quat = [w x y z];
figure
plot(z);
grid on
title("Yaw (Radians)")
xlabel("t(s)")
ylabel("Angle(rads)")
set(gca, 'FontSize', 24, 'FontName', 'Times')
%quat = int(quaternion);
%% convert quart to euler angles.

eulZYX = quat2eul(quat,'ZYX');

%% Convert Euler angles from radians to degrees
eulZYX_deg = rad2deg(eulZYX);
%% Plots
figure


plot(Xx,Yy,"LineWidth",4);
set(gca, 'FontSize', 24, 'FontName', 'Times')
xlabel("$x(m)$", 'Interpreter','latex')
ylabel("$y(m)$",'Interpreter','latex')
title("Trajectory",'Interpreter','latex')
grid on

%% plot x 
yaw = (eulZYX_deg(:,1));
pitch = (eulZYX_deg(:,2));
roll = (eulZYX_deg(:,3));

%% plots of data
figure;
subplot(3,1,1);
plot(yaw,"LineWidth",4)
%title("Yaw")

set(gca, 'FontSize', 12, 'FontName', 'Times')
xlabel("$t(s)$", 'Interpreter','latex')
ylabel("$Angle (deg. ^{\ \circ})$",'Interpreter','latex')
title("$Yaw$",'Interpreter','latex')
grid on
%%

subplot(3,1,2);
plot(pitch,"LineWidth",4)
%title("Pitch (deg)")

set(gca, 'FontSize', 12, 'FontName', 'Times')
xlabel("$t(s)$", 'Interpreter','latex')
ylabel("$Angle (deg. ^{\ \circ})$",'Interpreter','latex')
title("$Pitch$",'Interpreter','latex')
grid on
%%
subplot(3,1,3);
plot(roll,"LineWidth",4)
%title("Roll (deg)")

grid on
set(gca, 'FontSize', 12, 'FontName', 'Times')
xlabel("$t(s)$", 'Interpreter','latex')
ylabel("$Angle (deg. ^{\ \circ})$",'Interpreter','latex')
title("$Roll$",'Interpreter','latex')
%% save plot
%%print("Plot.svg",'-dsvg')

%% plots of data
figure;
subplot(3,1,1);
plot(x,"LineWidth",4)
%title("Pitch")

set(gca, 'FontSize', 12, 'FontName', 'Times')
xlabel("$t(s)$", 'Interpreter','latex')
ylabel("$Angle (deg. ^{\ \circ})$",'Interpreter','latex')
title("$Pitch$",'Interpreter','latex')
grid on
legend("Orientation around X axis") 
%%

subplot(3,1,2);
plot(y,"LineWidth",4)
%title("Yaw (deg)")

set(gca, 'FontSize', 12, 'FontName', 'Times')
xlabel("$t(s)$", 'Interpreter','latex')
ylabel("$Angle (deg. ^{\ \circ})$",'Interpreter','latex')
title("$Yaw$",'Interpreter','latex')
grid on
legend("Orientation around Y axis") 
%%
subplot(3,1,3);
plot(z,"LineWidth",4)
%title("Roll (deg)")

grid on
set(gca, 'FontSize', 12, 'FontName', 'Times')
xlabel("$t(s)$", 'Interpreter','latex')
ylabel("$Angle (deg. ^{\ \circ})$",'Interpreter','latex')
title("$Roll$",'Interpreter','latex')
legend("Orientation around Z axis") 

%%
%%

rollx = rad2deg(x);
pitchy = rad2deg(y);
yawz = rad2deg(z) * -1;

theta = deg2rad(37); % Convert angle to radians
R = [cos(theta) -sin(theta); sin(theta) cos(theta)]; % Rotation matrix
rotated_data = (R * [rdTrajectory.Xzero'; rdTrajectory.Yzero'])'; % Apply rotation to each point
Xr = rotated_data(:,1);
Yr = rotated_data(:,2);

figure;
subplot(2,1,1)
plot(Xr, Yr, 'LineWidth', 4)
title('$2nd Trajectory$', 'Interpreter', 'latex')
grid on
xlabel('$x(m)$', 'Interpreter', 'latex')
ylabel('$y(m)$', 'Interpreter', 'latex')

axis equal
grid on

% Optional: set figure and axes properties
% set(gcf, 'Position', [100 100 500 500]);
% set(gca, 'FontSize', 18, 'FontName', 'Times');
% set(gca, 'XMinorTick', 'on', 'YMinorTick', 'on');

subplot(2,1,2)
% Plot data for x, y, z with different line styles or colors
plot(t, rollx, 'LineWidth', 4, 'DisplayName', 'Roll'); hold on;
plot(t, pitchy, 'LineWidth', 4, 'DisplayName', 'Pitch');
plot(t, yawz, 'LineWidth', 4, 'DisplayName', 'Heading'); hold off;

% Set axes properties
set(gca, 'FontSize', 20, 'FontName', 'Times');
xlabel('$t(s)$', 'Interpreter', 'latex');
ylabel('$Angle (deg. ^{\ \circ})$', 'Interpreter', 'latex');
% title("Orientation Angles", 'Interpreter', 'latex');
grid on;

% Add a legend
legend show


