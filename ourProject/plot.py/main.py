import matplotlib.pyplot as plt

x_ticks = [0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000]
problem_size = [16, 64, 128, 256, 384, 512, 768, 1024, 1280, 1536, 2048, 2560, 3072, 3584, 3968]
basic_memory = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]  # Lila
basic_time = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]  # Lila
efficient_memory = [59801, 61440, 61849, 64307, 65126, 64716, 72499, 74956, 76595, 77824, 97484, 105676, 108953, 129843, 135987]
efficient_time = [0.104, 0.478, 1.232, 3.948, 8.419, 14.896, 29.204, 51.426, 75.839, 103.417, 183.508, 285.994, 408.459, 566.848, 703.171]

plt.plot(problem_size, basic_memory, '.-', label='Basic')
plt.plot(problem_size, efficient_memory, '.-', label='Efficient')
plt.xticks(x_ticks)  # 设置横坐标刻度为给定的年份
plt.xlabel('Problem Size (M+N)')  # 设置横坐标轴标题
plt.ylabel('Memory')
plt.title('Memory usage vs Problem size')
plt.legend()  # 显示图例，即每条线对应 label 中的内容
plt.savefig('./memory.png')
plt.show()  # 显示图形


# plt.plot(problem_size, basic_time, '.-', label='Basic')
# plt.plot(problem_size, efficient_time, '.-', label='Efficient')
# plt.xticks(x_ticks)  # 设置横坐标刻度为给定的年份
# plt.xlabel('Problem Size (M+N)')  # 设置横坐标轴标题
# plt.ylabel('Time')
# plt.title('CPU time vs Problem size')
# plt.title('Memory usage vs Problem size')
# plt.legend()  # 显示图例，即每条线对应 label 中的内容
# plt.savefig('./time.png')
# plt.show()  # 显示图形
