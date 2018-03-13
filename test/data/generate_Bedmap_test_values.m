% This script generates a CSV file containing 100 randomly selected
% lat lon xi yi surface ice_thickness bed bed_uncertainty icemask

% lat, lon is the latitude and longitude of the test point
% xi, yi is the index into the native BEDMAP2 data grid
% rock_thickness is the rock_thickness (in km) at the test point
% rock_thickness_uncertainty is the uncertainty in rock_thickness (in km)
% ice_thickness is the ice_thickness at the test point
%
% this requires the Antarctic Mapping Toolbox (AMT) and the Bedmap2 toolbox
% to function propertly; they are both freely available from Mathworks
%

% download the bedmap dataset again
bedmap2_download

% we want 500 random points
N = 1000;

% create an array to store our data
data = zeros(N, 9, 'double');

% loop over N, filling in the array
for i = 1:N
    
    % generate a random lat/lon - we don't care about correct
    % sphere point picking here
    lat = -double(unifrnd(61, 89));
    lon = double(unifrnd(-179, 179));
    
    % convert to BEDMAP2 coordinates
    [x,y] = ll2ps(lat, lon);
    
    % and save the values of the various datasets at this location
    % using linear interpolation
    data(i, 1) = lat;
    data(i, 2) = lon;
    data(i, 3) = x;
    data(i, 4) = y;
    data(i, 5) = bedmap2_interp(lat, lon, 'surfacew', 'linear');
    data(i, 6) = bedmap2_interp(lat, lon, 'thickness', 'linear');
    data(i, 7) = bedmap2_interp(lat, lon, 'bedw', 'linear');
    data(i, 8) = bedmap2_interp(lat, lon, 'beduncertainty', 'linear');
    data(i, 9) = bedmap2_interp(lat, lon, 'icemask', 'nearest');
    
end

% and write to a file
csvwrite('Bedmap_test_values.csv', data)

