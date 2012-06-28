memset = textread('memset_0_values.txt');
random = textread('memset_0_random_values.txt');
naive = textread('naive_values.txt');
x = (1:37)*10000;
plot(x, -memset(1:37), 'g', 'LineWidth', 2);
hold on; plot(x, -naive(1:37), 'r', 'LineWidth', 2);
plot(x, -random(1:37), 'b', 'LineWidth', 2);
legend ('random initial q', 'naive', 'zero initial q')
% xlabel('epochs')
% ylabel('performance (total wait)')
grid on;

 