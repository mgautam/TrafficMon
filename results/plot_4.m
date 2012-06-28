values_4 = textread('n_4_values.txt');
values_4_naive = textread('n_4_naive_values.txt');
x = (1:37)*10000;
plot(x, -values_4(1:37), 'b', 'LineWidth', 2);
hold on; 

plot(x, -values_4_naive(1:37), 'r', 'LineWidth', 2);

legend ('RL', 'Naive');
% xlabel('epochs')
% ylabel('performance (total wait)')
grid on;

 