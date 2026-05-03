%% Load CSV
data = readtable('step_response.csv');
time_col = datetime(data.Timestamp);
t = seconds(time_col - time_col(1));  % elapsed seconds from zero
setpoint = data.Setpoint;
measured_rpm = data.Measured_RPM;
pwm = data.PWM;

%% Compute error
error = setpoint - measured_rpm;

%% Settling detection (10% band)
tolerance = 0.05 * setpoint(end);  % 5% of final setpoint
within_band = abs(error) <= tolerance;
settling_index = NaN;
for i = 1:length(within_band)
    if all(within_band(i:end))
        settling_index = i;
        break;
    end
end

%% Plotting
figure;

% --- RPM vs Time ---
subplot(3,1,1);
plot(t, measured_rpm, 'b', 'LineWidth', 1.5); hold on;
plot(t, setpoint, '--r', 'LineWidth', 1.5);
if ~isnan(settling_index)
    plot(t(settling_index), measured_rpm(settling_index), 'ko', ...
        'MarkerFaceColor', 'g');
    xline(t(settling_index), '--k', 'Settling Time');
end
title('RPM vs Time');
xlabel('Time (s)');
ylabel('RPM');
legend('Measured RPM', 'Setpoint', 'Settling Point');
grid on;

% --- PWM vs Time ---
subplot(3,1,2);
plot(t, pwm, 'm', 'LineWidth', 1.5);
title('PWM vs Time');
xlabel('Time (s)');
ylabel('PWM');
grid on;

% --- Error vs Time ---
subplot(3,1,3);
plot(t, error, 'k', 'LineWidth', 1.5);
yline(0, '--r');
title('Error vs Time');
xlabel('Time (s)');
ylabel('Error (RPM)');
grid on;

%% Print settling time
if ~isnan(settling_index)
    fprintf('Settling Time = %.3f seconds\n', t(settling_index));
else
    fprintf('No settling detected within tolerance.\n');
end

%% Save plot as PNG
saveas(gcf, 'step_response.png');