
function a = irsensor_calibrate(input_data_name, output_calibration_name, output_picture_name_s, do_picture)

m = load(input_data_name, '-ascii');

%Assumes 100*45 measurements.
for n = 0:44
	m3(1+n, :) = sum(m((1+n*100):(100+n*100), :))/100;
end;

dist = 0.15:0.01:0.59;

if (do_picture) plot(dist, m3); end;

if (do_picture) print('-r72', '-dpng', 'irsensor/calibration_input_data'); end;

%First guess.
x0 = [16, 76];

[rows, columns] = size(m3);

for n = 1:columns
	a(n, :) = lsqcurvefit(@irdist, x0, dist', m3(:, n));
	if (do_picture)
		figure;
		plot(dist, m3(:, 1), 'xr', dist, irdist(a(1, :), dist), '-g');
		s = sprintf('irsensor/%s_%d', output_picture_name_s, n);
		print('-r72', '-dpng',  s);
	end;
end;

save(sprintf('irsensor/%s', output_calibration_name), 'a', '-ascii');
