library(ggplot2)
library(tidyr)

df <- read.csv("/home/thinh/Downloads/test_r0_3_diffWaitingTime.csv")
df$Day <- row.names(df)

df_plot <- gather(df, key = "Compartment", value = "Value", -ncol(df))
df_plot$Compartment <- factor(df_plot$Compartment)
df_plot$Day <- as.numeric(df_plot$Day)
str(df_plot)

ggplot(df_plot, aes(x = Day, y = Value, color = Compartment)) + geom_line()
df$sum <- rowSums(df[,-ncol(df)])
ggsave("/home/thinh/Dropbox/oucru/cpp/cpp_training/plots/test_r0_3_diffWaitingTime.png", width = 7, height = 3)
