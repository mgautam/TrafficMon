g01 = textread('gamma0_1rand_values.txt');
g02 = textread('gamma0_2rand_values.txt');
g04 = textread('gamma0_4rand_values.txt');
g06 = textread('gamma0_6rand_values.txt');
g08 = textread('gamma0_8rand_values.txt');
g10 = textread('gamma1_0rand_values.txt');

x = (1:37)*10000;
plot(x, -g01(1:37), 'b', 'LineWidth', 2);
hold on;

plot(x, -g02(1:37), 'm', 'LineWidth', 2);

% plot(x, -g04(1:37), 'r', 'LineWidth', 2);

plot(x, -g06(1:37), 'g', 'LineWidth', 2);

% plot(x, -g08(1:37), 'm', 'LineWidth', 2);

plot(x, -g10(1:37), 'r', 'LineWidth', 2);

legend ('gamma = 0.1', ...
        'gamma = 0.2', ...
        'gamma = 0.6', ...
        'gamma = 1.0');

grid on;