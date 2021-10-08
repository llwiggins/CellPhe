function [boundaries] = ROIs2Coordinates(cvsROIs)

% ROIs2Coordinates - FUNCTION Convert a set of imported ImageJ ROIs into cell boundaries
%
% 'cvsROIs' is a cell array of ImageJ ROI structures, as imported by
% ReadImageJROI. 
%
% 'Coords' will contain one cell number, frame number, and set of coordinates
% for each compatible ROI in 'cvsROIs'.
%


max = 0;
for (nROIIndex = numel(cvsROIs):-1:1)
   sThisROI = cvsROIs{nROIIndex};
   Coords.PixelList{nROIIndex} = sThisROI.mnCoordinates;
   length = size(Coords.PixelList{nROIIndex});
   if (length(1) > max) max = length(1);
     end
  end

%boundaries = zeros(numel(cvsROIs)+1,(2*max)+3, 'double');
boundaries(1,1) = max;
for (nROIIndex = numel(cvsROIs):-1:1)
    sThisROI = cvsROIs{nROIIndex};
    % nPosition is the frame number
    boundaries(nROIIndex+1,1) = sThisROI.nPosition;
    % strName is the cell number
    boundaries(nROIIndex+1,2) = str2num(sThisROI.strName);
    length = size(Coords.PixelList{nROIIndex});
    boundaries(nROIIndex+1,3) = length(1);
    x =  Coords.PixelList{nROIIndex};
    for (i = 1:1:length(1))
        boundaries(nROIIndex+1,(2*i)+2) = x(i,1);
        boundaries(nROIIndex+1,(2*i)+3) = x(i,2);        
     end
  end

end

% --- END of ROIs2Coordinates.m ---
