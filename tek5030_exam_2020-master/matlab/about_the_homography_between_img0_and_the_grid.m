% Compute the homography between the grid and image from point correspondences.
img_points = [102,911; 1660, 878; 885, 52; 1323, 68];
grid_points = [1,2; 3,2; 2,0; 3,0];
tform = fitgeotrans(grid_points, img_points, 'projective');

% Define the grid boundary.
x_min = 0;
x_max = 4;
y_min = 0;
y_max = 5;

% Plot the grid in image coordinates.
% This looks strange, because some of the points on the grid boundary
% lies on the other side of the image plane, and is therefore reflected
% to the other side of the vanishing point.
figure;
hold on;
for x=x_min:x_max
  [u, v] = transformPointsForward(tform, [x, x], [y_min, y_max]);
  plot(u, v, 'r');
end
for y=y_min:y_max
  [u, v] = transformPointsForward(tform, [x_min, x_max], [y, y]);
  plot(u, v, 'r');
end
axis ij;
axis equal;

% We can illustrate this by computing the intersection between
% the grid plane and the imaging plane.
% We do this by transforming the line at infinity in image coordinates
% into the grid coordinate system using the homography.

% Get the homography from image to grid coordinates.
H_gi = invert(tform).T';

% The line at infinity in image coordinates is given by:
l_inf = [0,0,1]';

% Transform the line at infinity to grid coordinates using the homography.
l_inf_g = inv(H_gi)' * l_inf;

% Extract two points on the line (a little bit outside the grid boundary).
x0 = x_min - 1;
x1 = x_max + 1;
y0 = - (l(1)*x0 + l(3))/l(2);
y1 = - (l(1)*x1 + l(3))/l(2);

% Plot the grid in red.
figure;
hold on;
for x=x_min:x_max
  plot([x, x], [y_min, y_max], 'r');
end
for y=y_min:y_max
  plot([x_min, x_max], [y, y], 'r');
end

% Plot where the image plane intersects the grid plane in blue.
plot([x0, x1], [y0, y1], 'b');

axis ij;
axis equal;
